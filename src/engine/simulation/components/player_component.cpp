//
// Created by Ion Agorria on 13/06/19
//
#include "player_component.h"

CLASS_COMPONENT_DEFAULT(PlayerComponent)

void PlayerComponent::simulationChanged() {
}

void PlayerComponent::update() {
}

void PlayerComponent::draw() {
}

Player* PlayerComponent::getPlayer() {
    return player;
}

void PlayerComponent::setPlayer(Player* newPlayer) {
    this->player = newPlayer;
}