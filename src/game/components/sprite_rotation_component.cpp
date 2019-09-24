//
// Created by Ion Agorria on 08/09/19
//

#include "engine/simulation/components/attachment_component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/entities/entity_config.h"
#include "game/core/game.h"
#include "sprite_rotation_component.h"

void SpriteRotationComponentCommon::updateSpriteIndex(Entity* base) {
    //Check if anything changed to update sprite
    uint16_t rotationIndexNew = 0;
    ImageComponent* imageComponent = GET_COMPONENT(base, ImageComponent);
    number_t spriteAngleHalf = base->getConfig()->getData("sprite_angle_half");
    bool ccw = Game::angleToSpriteIndex(base->getDirection(), spriteAngleHalf, rotationIndexNew);
    if (spriteIndex != rotationIndexNew) {
        //Setup stuff
        spriteIndex = rotationIndexNew;
        uint16_t spriteSides = base->getConfig()->getData("sprite_sides");
        if (spriteIndex == 0) {
            ccw = false;
        } else if (spriteIndex >= spriteSides) {
            ccw = false;
            spriteIndex = spriteSides;
        }
        //Update sprite direction
        number_t spriteAngle = base->getConfig()->getData("sprite_angle");
        spriteDirection = Game::angleToSpriteAngle(base->getDirection(), spriteAngle);
        imageComponent->imageFlipX = ccw;
        //Select the sprite of current index by calling chooseSprite
        chooseSprite();
        //Update attached entities if requested
        AttachmentComponent* attachmentComponent = GET_COMPONENT(base, AttachmentComponent);
        if (attachmentComponent && !attachmentComponent->updateAttachmentOnEntityChange) {
            attachmentComponent->updateAttachmentPositions(spriteDirection);
        }
    }
}

CLASS_COMPONENT_DEFAULT(SpriteRotationComponent)

void SpriteRotationComponent::update() {
}

void SpriteRotationComponent::setup() {
}

void SpriteRotationComponent::simulationChanged() {
    if (base->isActive()) {
        updateSpriteIndex(base);
    }
}

void SpriteRotationComponent::entityChanged() {
    updateSpriteIndex(base);
}

void SpriteRotationComponent::chooseSprite() {
    ImageComponent* imageComponent = GET_COMPONENT(base, ImageComponent);
    imageComponent->setImageFromSprite("default_" + std::to_string(spriteIndex));
}
