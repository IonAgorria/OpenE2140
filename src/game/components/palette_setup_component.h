//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_PLAYER_SETUP_COMPONENT_H
#define OPENE2140_PLAYER_SETUP_COMPONENT_H

#include "game/core/constants.h"
#include "engine/simulation/components/component.h"
#include "engine/simulation/components/player_component.h"

/**
 * Handles palette setup per entity type
 */
class PaletteSetupComponent {
CLASS_COMPONENT_BODY(Entity, PaletteSetupComponent)
public:
    /**
     * Destructor
     */
    ~PaletteSetupComponent() = default;

    void construction() {
    }

    void setup() {
        //Set palette size using the entity config data
        size_t lowestEntry = 0;
        const EntityConfig* config = base->getConfig();
        switch (config->kind) {
            case ENTITY_KIND_UNIT:
                lowestEntry = PALETTE_UNIT_MOVEMENT0;
                break;
            case ENTITY_KIND_BUILDING:
                //Some buildings don't use certain indexes
                if (config->code == "water_base") {
                    lowestEntry = PALETTE_BUILDING_PLAYER0;
                } else {
                    lowestEntry = PALETTE_BUILDING_LIGHT0;
                }
                break;

        }

        //Setup palette size
        if (0 < lowestEntry) {
            ImageComponent* animationComponent = GET_COMPONENT(base, ImageComponent);
            animationComponent->extraPaletteSize = 0x100 - lowestEntry;
        }
    }

    void simulationChanged() {
        if (!base->isActive()) {
            return;
        }
        //ImageComponent* imageComponent = GET_COMPONENT(base, ImageComponent);
        //Palette* palette = imageComponent->getExtraPalette();

        //Load player color
        /* TODO set color to palette
        PlayerComponent* playerComponent = GET_COMPONENT(base, PlayerComponent);
        if (playerComponent) {
            Player* player = playerComponent->getPlayer();
            if (player) {
            }
        }
        */
    }

    void update() {}

    void draw() {}
};;

#endif //OPENE2140_PLAYER_SETUP_COMPONENT_H
