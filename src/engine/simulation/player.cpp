//
// Created by Ion Agorria on 3/08/19
//

#include "faction.h"
#include "player.h"

Player::Player(player_id_t id):
    id(id), mask(BIT_MASK(id)) {
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