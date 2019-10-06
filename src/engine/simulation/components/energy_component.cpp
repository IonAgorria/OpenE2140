//
// Created by Ion Agorria on 15/9/19
//
#include "engine/simulation/player.h"
#include "engine/simulation/entity.h"
#include "engine/entities/entity_config.h"
#include "player_component.h"
#include "energy_component.h"

CLASS_COMPONENT_DEFAULT(EnergyComponent)

void EnergyComponent::setup() {
}

void EnergyComponent::simulationChanged() {
    if (base->isActive()) {
        const EntityConfig* config = base->getConfig();
        energyGeneration = config->getData<entity_energy_t>("energy_generation", 0);
        energyRequirement = config->getData<entity_energy_t>("energy_requirement", 0);
    }
}

void EnergyComponent::update() {
    bool satisfied = false;

    //Get player
    PlayerComponent* playerComponent = GET_COMPONENT(base, PlayerComponent);
    Player* player = playerComponent ? playerComponent->getPlayer() : nullptr;
    if (player) {
        //Add to player energy pool
        if (0 < energyGeneration) {
            player->energyGeneration += energyGeneration;
        }

        //Use energy from player energy pool
        satisfied = player->energyPool >= energyRequirement;
        if (0 < energyRequirement && satisfied) {
            player->energyPool -= energyRequirement;
        }
    }

    //Set state
    if (energySatisfied != satisfied) {
        base->changesCount++;
        energySatisfied = satisfied;
    }
}

void EnergyComponent::entityChanged() {
}