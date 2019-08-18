//
// Created by Ion Agorria on 13/06/19
//
#include "engine/simulation/world/world.h"
#include "object.h"

void Object::simulationChanged() {
    if (isActive()) {
        //Set bounds
        World* world = simulation->getWorld();
        bounds.setCenter(position, Vector2(world->tileSize));

        //Load image according to tileset
        setImageFromSprite("default_" + std::to_string(world->tilesetIndex));
    }

    Entity::simulationChanged();
}

void Object::draw() {
    ImageComponent::draw(renderer);
}

void Spinner::simulationChanged() {
    Object::simulationChanged();

    if (isActive()) {
        //Setup the animation
        setAnimationFromSprite("default");
    }
}

void Spinner::update() {
    Entity::update();

    //Check if animation finished, flip the image and reverse the animation
    if (animation && animation->isFinished()) {
        animation->reverse = !animation->reverse;
        imageFlipX = clockwise == animation->reverse;
    }
}