//
// Created by Ion Agorria on 08/08/19
//
#include "src/engine/entities/entity_config.h"
#include "engine/graphics/animation.h"
#include "image_component.h"

CLASS_COMPONENT_DEFAULT(ImageComponent)

void ImageComponent::setup() {
}

void ImageComponent::simulationChanged() {
}

void ImageComponent::update() {
    if (animationPlay && animation) {
        if (animation->update(GAME_DELTA)) {
            image = animation->getCurrentFrame();
            if (image) {
                image->getRectangle().getSize(imageSize);
            }
        }
    }
}

void ImageComponent::entityChanged() {
}

void ImageComponent::draw(Renderer* renderer) {
    if (image) {
        Vector2 position = base->getPosition();
        position += imageOffset;
        Vector2 size = imageSize;
        if (imageFlipX) size.x *= -1;
        if (imageFlipY) size.y *= -1;
        if (imageCentered) {
            renderer->drawImageCenter(position, size, imageDirection, *image, extraPalette.get());
        } else {
            renderer->drawImage(position, size, *image, extraPalette.get());
        }

        /*
        Rectangle rect(position, size);
        if (imageCentered) rect.setCenter(position, size);
        renderer->drawRectangle(rect, 1, Color::GREEN);
        //*/
    }
}

Image* ImageComponent::getImage() const {
    return image;
}

Palette* ImageComponent::getImagePalette() const {
    return image ? image->getPalette().get() : nullptr;
}

Animation* ImageComponent::getAnimation() const {
    return animation.get();
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