//
// Created by Ion Agorria on 22/06/19
//
#include "engine/simulation/entities/entity_config.h"
#include "image_component.h"

CLASS_COMPONENT_DEFAULT(ImageComponent)

void ImageComponent::setup() {
}

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

void ImageComponent::setImageFromSprite(const std::string& code) {
    const EntityConfig* config = base->getConfig();
    SpriteGroup* group = config ? config->getSprite(code) : nullptr;
    if (group && !group->images.empty()) {
        image = group->images.at(0);
        if (image) {
            image->getRectangle().getSize(imageSize);
        }
    }
}
