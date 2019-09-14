//
// Created by Ion Agorria on 13/06/19
//
#include "src/engine/entities/entity_config.h"
#include "engine/simulation/world/world.h"
#include "attachment.h"

void Spinner::simulationChanged() {
    if (isActive()) {
        //Setup the animation
        setAnimationFromSprite("default");
        if (animation) {
            animation->reverse = !clockwise;
        }
    }

    Entity::simulationChanged();
}

void Spinner::update() {
    Entity::update();
    if (!animation) {
        return;
    }

    //Check if animation finished, flip the image and reverse the animation
    if (animation->isFinished()) {
        animation->reverse = !animation->reverse;
    }
    imageFlipX = clockwise == animation->reverse;

    //Some sprites need compensation of X by 1 pixel
    if (config->code == "flywheel_mine") {
        size_t index = animation->getCurrentIndex();
        if (index < 2) {
            imageOffset.x = imageFlipX ? -1 : 1;
        } else {
            imageOffset.x = 0;
        }
    }
}

void Spinner::draw() {
    ImageComponent::draw(renderer);
}

void ConveyorBelt::simulationChanged() {
    if (isActive()) {
        ImageComponentSlotted<0>::setImageFromSprite("head");
        ImageComponentSlotted<1>::setAnimationFromSprite("body");
        if (parent) {
            PaletteComponent* paletteComponent = GET_COMPONENT(parent, PaletteComponent);
            ImageComponentSlotted<0>::extraPalette = paletteComponent->getPalette();
            ImageComponentSlotted<1>::extraPalette = paletteComponent->getPalette();
        }
    }
    Entity::simulationChanged();
}

void ConveyorBelt::update() {
    Entity::update();
}

void ConveyorBelt::draw() {
    ImageComponentSlotted<0>::draw(renderer);
    ImageComponentSlotted<1>::draw(renderer);
}

void BuildingExit::update() {
    Entity::update();
}

void BuildingExit::draw() {
    ImageComponent::draw(renderer);
}

void BuildingExitUnderground::update() {
    Entity::update();
}

void BuildingExitUnderground::draw() {
    ImageComponentSlotted<0>::draw(renderer);
    ImageComponentSlotted<1>::draw(renderer);
    ImageComponentSlotted<2>::draw(renderer);
}

void Turret::simulationChanged() {
    if (isActive()) {
        if (parent) {
            PaletteComponent* paletteComponent = GET_COMPONENT(parent, PaletteComponent);
            ImageComponent::extraPalette = paletteComponent->getPalette();
        }
    }
    Entity::simulationChanged();
}

void Turret::update() {
    Entity::update();
}

void Turret::draw() {
    ImageComponent::draw(renderer);
}