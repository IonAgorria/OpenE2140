//
// Created by Ion Agorria on 15/8/19
//

#include "engine/simulation/player.h"
#include "engine/simulation/entities/entity_config.h"
#include "engine/simulation/components/player_component.h"
#include "engine/simulation/components/image_component.h"
#include "palette_component.h"

void PaletteComponent::construction() {
}

unsigned short test;
void PaletteComponent::update() {
    test+=10;
    setLight(0x8000 < test);
}

void PaletteComponent::setup() {
    const EntityConfig* config = base->getConfig();

    //Copy flags from config
    lowestEntry = config->getData<size_t>("palette_lowest_entry", lowestEntry);
    if (0 == lowestEntry) {
        return;
    }
    hasPlayer = config->getData<bool>("palette_player", false);
    hasLight = config->getData<bool>("palette_light", false);
    hasShadow = config->getData<bool>("palette_shadow", false);
    hasMovement = config->getData<bool>("palette_movement", false);
    hasFire = config->getData<bool>("palette_fire", false);

    //Create palette and set it to image component
    ImageComponent* imageComponent = GET_COMPONENT(base, ImageComponent);
    palette = std::make_shared<Palette>((PALETTE_MAX_INDEX+1) - lowestEntry, true);
    imageComponent->extraPalette = palette;
}

void PaletteComponent::simulationChanged() {
    if (!base->isActive()) {
        return;
    }
    if (!palette) {
        return;
    }
    const EntityConfig* config = base->getConfig();

    //Copy the original colors since some entities might not have lights or other stuff
    ImageComponent* imageComponent = GET_COMPONENT(base, ImageComponent);
    Palette* imagePalette = imageComponent->getImagePalette();
    if (imagePalette) {
        palette->setColors(imagePalette, lowestEntry, 0, palette->length() - 1);
    }

    //Load player color
    PlayerComponent* playerComponent = GET_COMPONENT(base, PlayerComponent);
    if (playerComponent) {
        Player* player = playerComponent->getPlayer();
        if (player) {
            auto colors = player->extraColors;
            for (size_t i = 0; i < PALETTE_PLAYER_AMOUNT && i < colors.size(); ++i) {
                palette->setColor(PALETTE_PLAYER - lowestEntry + i, colors[i]);
            }
        }
    }

    //Setup the rest
    setupShadows(config);
    palette->setColor(0xFF - lowestEntry, Color::BLACK);
    palette->updateTexture();
}

void PaletteComponent::setupShadows(const EntityConfig* config) {
    size_t shadowMain = 0;
    size_t shadowExtra = 0;
    switch (config->kind) {
        case ENTITY_KIND_OBJECT:
            shadowMain = PALETTE_OBJECT_SHADOW;
            break;
        case ENTITY_KIND_UNIT:
            shadowMain = PALETTE_UNIT_SHADOW;
            break;
        case ENTITY_KIND_BUILDING:
            shadowMain = PALETTE_BUILDING_SHADOW;
            shadowExtra = PALETTE_BUILDING_SHADOW_EXTRA;
            break;

    }

    if (shadowMain) {
        palette->setColor(shadowMain - lowestEntry, Color::SHADOW_MAIN);
    }
    if (shadowExtra) {
        palette->setColor(shadowExtra - lowestEntry, Color::SHADOW_EXTRA);
    }
}

void PaletteComponent::setLight(bool state) {
    if (!palette || !hasLight) {
        return;
    }
    const EntityConfig* config = base->getConfig();
    switch (config->kind) {
        case ENTITY_KIND_UNIT:
            //TODO get color for this
            palette->setColor(
                    PALETTE_UNIT_LIGHT - lowestEntry,
                    state ? Color::BLUE : Color::BLACK
            );
            break;
        case ENTITY_KIND_BUILDING:
            //Some buildings don't use certain indexes
            if (config->code == "water_base") {
                return;
            }
            palette->setColor(
                    PALETTE_BUILDING_LIGHT0 - lowestEntry,
                    state ? Color::BUILDING_LIGHT0_ON : Color::BUILDING_LIGHT_OFF
            );
            palette->setColor(
                    PALETTE_BUILDING_LIGHT1 - lowestEntry,
                    state ? Color::BUILDING_LIGHT1_ON : Color::BUILDING_LIGHT_OFF
            );
            palette->setColor(
                    PALETTE_BUILDING_LIGHT2 - lowestEntry,
                    state ? Color::BUILDING_LIGHT2_ON : Color::BUILDING_LIGHT_OFF
            );
            palette->setColor(
                    PALETTE_BUILDING_LIGHT3 - lowestEntry,
                    state ? Color::BUILDING_LIGHT3_ON : Color::BUILDING_LIGHT_OFF
            );
            break;
        default:
            //Nothing to do
            return;
    }
    palette->updateTexture();
}

Palette* PaletteComponent::getPalette() const {
    return palette.get();
}