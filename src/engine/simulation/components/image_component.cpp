//
// Created by Ion Agorria on 08/08/19
//
#include "engine/simulation/entities/entity_config.h"
#include "engine/graphics/animation.h"
#include "image_component.h"

CLASS_COMPONENT_DEFAULT(ImageComponent)

void ImageComponent::setup() {
    if (0 < extraPaletteSize) {
        extraPalette = std::make_unique<Palette>(extraPaletteSize, true);
    }
}

void ImageComponent::simulationChanged() {
}

void ImageComponent::update() {
    if (animationPlay && animation) {
        animation->update(GAME_DELTA);
        image = animation->getCurrentFrame();
        if (image) {
            image->getRectangle().getSize(imageSize);
        }
    }
}

void ImageComponent::draw(Renderer* renderer) {
    if (image) {
        Vector2 position = base->getPosition();
        position += imageOffset;
        renderer->drawImage(position, imageSize, imageDirection, *image, nullptr);
    }
}

Animation* ImageComponent::getAnimation() const {
    return animation.get();
}

Palette* ImageComponent::getExtraPalette() const {
    return extraPalette.get();
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

void ImageComponent::setAnimationFromSprite(const std::string& code, bool restart) {
    const EntityConfig* config = base->getConfig();
    SpriteGroup* group = config ? config->getSprite(code) : nullptr;
    if (group) {
        animation = std::make_unique<Animation>();
        animation->duration = group->duration;
        animation->loop = group->loop;
        animation->setFrames(group->images);
        if (restart) {
            animation->setCurrentIndex(0);
        }
    }
}