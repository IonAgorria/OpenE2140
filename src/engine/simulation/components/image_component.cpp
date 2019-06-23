//
// Created by Ion Agorria on 22/06/19
//
#include "image_component.h"
#include "player_component.h"

CLASS_COMPONENT_DEFAULT(ImageComponent)

void ImageComponent::simulationChanged() {
}

void ImageComponent::update() {
}

void ImageComponent::draw(Renderer* renderer) {
    if (image) {
        Vector2 position = base->getPosition();
        position += imageOffset;
        renderer->draw(position, imageSize, imageDirection, *image, nullptr);
    }
}