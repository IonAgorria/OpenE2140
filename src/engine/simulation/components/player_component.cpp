//
// Created by Ion Agorria on 13/06/19
//
#include "player_component.h"

CLASS_COMPONENT_DEFAULT(PlayerComponent)

void PlayerComponent::setup() {
}

void PlayerComponent::simulationChanged() {
}

void PlayerComponent::update() {
}

void PlayerComponent::entityChanged() {
}

Player* PlayerComponent::getPlayer() {
    return player;
}

void PlayerComponent::setPlayer(Player* newPlayer) {
    this->player = newPlayer;
}