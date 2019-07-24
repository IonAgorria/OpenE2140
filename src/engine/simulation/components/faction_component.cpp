//
// Created by Ion Agorria on 12/07/19
//
#include <src/engine/io/log.h>
#include "faction_component.h"
#include "engine/simulation/entities/entity.h"
#include "engine/simulation/entities/entity_config.h"

CLASS_COMPONENT_DEFAULT(FactionComponent)

void FactionComponent::setup() {
    const std::string factionStr = base->getConfig()->getData<const std::string>("faction", "");
    Log::get()->debug(factionStr);
}

void FactionComponent::simulationChanged() {
}

void FactionComponent::update() {
}

Faction* FactionComponent::getFaction() {
    return faction;
}

void FactionComponent::setFaction(Faction* newFaction) {
    this->faction = newFaction;
}