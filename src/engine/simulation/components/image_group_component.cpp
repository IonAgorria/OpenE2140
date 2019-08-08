//
// Created by Ion Agorria on 08/08/19
//
#include "image_group_component.h"

CLASS_COMPONENT_DEFAULT(ImageGroupComponent)

void ImageGroupComponent::setup() {
    if (hasExtraPalette) {
        extraPalette = std::make_unique<Palette>();
    }
}

void ImageGroupComponent::simulationChanged() {
}

void ImageGroupComponent::update() {
}

void ImageGroupComponent::draw(Renderer* renderer) {
    if (image) {
        Vector2 position = base->getPosition();
        position += imageOffset;
        renderer->draw(position, imageSize, imageDirection, *image, nullptr);
    }
}