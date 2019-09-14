//
// Created by Ion Agorria on 08/09/19
//

#include "engine/simulation/components/image_component.h"
#include "engine/entities/entity_config.h"
#include "game/core/constants.h"
#include "game/core/game.h"
#include "sprite_rotation_component.h"

void SpriteRotationComponentCommon::updateSpriteRotation(Entity* base) {
    //Check if anything changed to update sprite
    uint16_t rotationIndexNew = 0;
    ImageComponent* imageComponent = GET_COMPONENT(base, ImageComponent);
    number_t spriteAngle = base->getConfig()->getData("sprite_angle");
    bool ccw = Game::angleToSpriteIndex(base->getDirection(), spriteAngle, rotationIndexNew);
    if (rotationIndex != rotationIndexNew) {
        rotationIndex = rotationIndexNew;
        uint16_t spriteSides = base->getConfig()->getData("sprite_sides");
        if (rotationIndex == 0) {
            ccw = false;
        } else if (rotationIndex >= spriteSides) {
            ccw = false;
            rotationIndex = spriteSides;
        }
        imageComponent->imageFlipX = ccw;
        chooseSprite();
    }
}

CLASS_COMPONENT_DEFAULT(SpriteRotationComponent)

void SpriteRotationComponent::update() {
}

void SpriteRotationComponent::setup() {
}

void SpriteRotationComponent::simulationChanged() {
    if (base->isActive()) {
        updateSpriteRotation(base);
    }
}

void SpriteRotationComponent::entityChanged() {
    updateSpriteRotation(base);
}

void SpriteRotationComponent::chooseSprite() {
    ImageComponent* imageComponent = GET_COMPONENT(base, ImageComponent);
    imageComponent->setImageFromSprite("default_" + std::to_string(rotationIndex));
}
