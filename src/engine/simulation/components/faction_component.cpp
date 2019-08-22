//
// Created by Ion Agorria on 12/07/19
//
#include "src/engine/entities/entity_config.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/player.h"
#include "player_component.h"
#include "faction_component.h"

CLASS_COMPONENT_DEFAULT(FactionComponent)

void FactionComponent::setup() {
}

void FactionComponent::simulationChanged() {
    if (base->isActive()) {
        PlayerComponent* playerComponent = castBase<PlayerComponent>();
        if (!faction && playerComponent) {
            Player* player = playerComponent->getPlayer();
            faction = player ? player->faction : nullptr;
        }
        if (!faction) {
            const std::string factionCode = base->getConfig()->getData<const std::string>("faction", "");
            if (!factionCode.empty()) {
                faction = base->getSimulation()->getFaction(factionCode);
            }
        }
    }
}

void FactionComponent::update() {
}

Faction* FactionComponent::getFaction() {
    return faction;
}

void FactionComponent::setFaction(Faction* newFaction) {
    this->faction = newFaction;
}