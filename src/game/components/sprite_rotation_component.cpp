//
// Created by Ion Agorria on 08/09/19
//

#include "engine/simulation/components/image_component.h"
#include "game/core/game.h"
#include "sprite_rotation_component.h"

void SpriteRotationComponentCommon::updateSpriteRotation(Entity* base) {
    //Check if anything changed to update sprite
    uint16_t rotationIndexNew = 0;
    ImageComponent* imageComponent = GET_COMPONENT(base, ImageComponent);
    imageComponent->imageFlipX = Game::angleToSpriteIndex(base->getDirection(), rotationIndexNew);
    if (rotationIndex != rotationIndexNew) {
        rotationIndex = rotationIndexNew;
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
