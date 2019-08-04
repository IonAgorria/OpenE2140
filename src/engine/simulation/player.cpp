//
// Created by Ion Agorria on 3/08/19
//

#include "faction.h"
#include "player.h"

Player::Player(player_id_t id, Faction* faction, player_mask_t enemies):
    id(id), mask(BIT_MASK(id)), faction(faction), enemies(enemies) {
}

Player::~Player() {
}

Faction* Player::getFaction() {
    return faction;
}

bool Player::isEnemy(const Player* other) const {
    if (!other) return false;
    return BIT_STATE(enemies, other->mask);
}

void Player::addEnemy(const Player* other) {
    BIT_ON(enemies, other->mask);
}

void Player::removeEnemy(const Player* other) {
    BIT_OFF(enemies, other->mask);
}