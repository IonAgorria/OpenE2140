//
// Created by Ion Agorria on 08/08/19
//
#include "engine/simulation/entities/entity_config.h"
#include "engine/graphics/animation.h"
#include "animation_component.h"

CLASS_COMPONENT_DEFAULT(AnimationComponent)

void AnimationComponent::setup() {
    animation = std::make_unique<Animation>();
    if (0 < extraPaletteSize) {
        extraPalette = std::make_unique<Palette>(extraPaletteSize, true);
    }
}

void AnimationComponent::simulationChanged() {
}

void AnimationComponent::update() {
    if (animation) {
        animation->update(GAME_DELTA);
        image = animation->getCurrentFrame();
        if (image) {
            image->getRectangle().getSize(imageSize);
        }
    }
}

void AnimationComponent::draw(Renderer* renderer) {
    if (image) {
        Vector2 position = base->getPosition();
        position += imageOffset;
        renderer->draw(position, imageSize, imageDirection, *image, nullptr);
    }
}

Animation* AnimationComponent::getAnimation() const {
    return animation.get();
}

Palette* AnimationComponent::getExtraPalette() const {
    return extraPalette.get();
}

void AnimationComponent::setAnimationFromSprite(const std::string& code, bool restart) {
    const EntityConfig* config = base->getConfig();
    SpriteGroup* group = config ? config->getSprite(code) : nullptr;
    if (group) {
        animation->duration = group->duration;
        animation->loop = group->loop;
        animation->setFrames(group->images);
        if (restart) {
            animation->setCurrentIndex(0);
        }
    }
}