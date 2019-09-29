//
// Created by Ion Agorria on 13/06/19
//
#include "src/engine/entities/entity_config.h"
#include "engine/simulation/world/world.h"
#include "attachment.h"

void Spinner::simulationChanged() {
    if (isActive()) {
        applyCorrection = config->code == "flywheel_mine";
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
    if (applyCorrection) {
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
        ImageComponentSlotted<0>::imageCentered = false;
        ImageComponentSlotted<1>::imageCentered = false;
        if (parent) {
            PaletteComponent* paletteComponent = GET_COMPONENT(parent, PaletteComponent);
            ImageComponentSlotted<0>::extraPalette = paletteComponent->getPalette();
        } else {
            BUG(toString() + " no parent set?");
        }

        //Calculate the offset to align the top left corner and set the head
        Vector2 size(bounds.w / -2, bounds.h / -2);
        ImageComponentSlotted<0>::imageOffset = size;

        //Set the body
        config->getVector2("body_alignment", ImageComponentSlotted<1>::imageOffset);
        ImageComponentSlotted<1>::imageOffset += size;
        this->setRunning(false);
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

void ConveyorBelt::setDirection(bool left) {
    auto& animation = ImageComponentSlotted<1>::animation;
    if (animation) {
        animation->reverse = left;
    }
}

void ConveyorBelt::setRunning(bool state) {
    ImageComponentSlotted<1>::animationPlay = state;
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
        } else {
            BUG(toString() + " no parent set?");
        }
    }
    Entity::simulationChanged();
}

void Turret::update() {
    Entity::update();
    setDirection(number_add(getDirection(), float_to_number(-0.010)));
}

void Turret::draw() {
    ImageComponent::draw(renderer);
}