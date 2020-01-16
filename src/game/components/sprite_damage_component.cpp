//
// Created by Ion Agorria on 08/09/19
//

#include "engine/entities/entity_config.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/world/world.h"
#include "sprite_damage_component.h"

CLASS_COMPONENT_DEFAULT(SpriteDamageComponent)

void SpriteDamageComponent::update() {
}

void SpriteDamageComponent::setup() {
}

void SpriteDamageComponent::simulationChanged() {
    if (base->isActive() && base->hasHealth()) {
        const EntityConfig* config = base->getConfig();
        damageHealth = 0;
        if (config) {
            damageHealth = config->getData<entity_health_t>("damage_health", base->getMaxHealth() / 2);
        }
    }
}

void SpriteDamageComponent::entityChanged() {
    chooseSprite();
}

void SpriteDamageComponent::chooseSprite() {
    ImageComponent* imageComponent = GET_COMPONENT_DYNAMIC(base, ImageComponent);

    //Load default sprites
    std::string code = "default";
    if (base->getCurrentHealth() < damageHealth) {
        code = "damaged";
    }
    //Load tileset if required
    const EntityConfig* config = base->getConfig();
    Simulation* simulation = base->getSimulation();
    if (config && config->hasVariants && simulation) {
        World* world =  simulation->getWorld();
        code += "_" + std::to_string(world->tilesetIndex);
    }

    imageComponent->setImageFromSprite(code);
}