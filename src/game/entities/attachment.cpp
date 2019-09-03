//
// Created by Ion Agorria on 13/06/19
//
#include "src/engine/entities/entity_config.h"
#include "engine/simulation/world/world.h"
#include "attachment.h"

void Attachment::simulationChanged() {
    if (isActive()) {
        //Make it disabled by default
        setDisable(true);
        setSelectable(false);

        //Set bounds
        Vector2 size;
        config->getVector2("size", size);
        bounds.setCenter(position, size);
    }
}

void Attachment::update() {
    bounds.setCenter(position);
    Entity::update();
}

void Attachment::draw() {
    ImageComponent::draw(renderer);
}

void Spinner::simulationChanged() {
    if (isActive()) {
        //Setup the animation
        setAnimationFromSprite("default");
        animation->reverse = !clockwise;
    }

    Attachment::simulationChanged();
}

void Spinner::update() {
    Attachment::update();
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