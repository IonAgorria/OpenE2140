//
// Created by Ion Agorria on 13/06/19
//
#include "engine/simulation/world/world.h"
#include "attachment.h"

void Attachment::simulationChanged() {
}

void Attachment::draw() {
    ImageComponent::draw(renderer);
}

void Spinner::simulationChanged() {
    if (isActive()) {
        //Set bounds
        World* world = simulation->getWorld();
        bounds.setCenter(position, Vector2(world->tileSize));

        //Setup the animation
        setAnimationFromSprite("default");
    }

    Attachment::simulationChanged();
}

void Spinner::update() {
    Entity::update();

    //Check if animation finished, flip the image and reverse the animation
    if (animation && animation->isFinished()) {
        animation->reverse = !animation->reverse;
        imageFlipX = clockwise == animation->reverse;
    }
}