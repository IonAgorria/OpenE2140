//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_PLAYER_COMPONENT_H
#define OPENE2140_PLAYER_COMPONENT_H

#include "component.h"

class Entity;
class Player;

/**
 * Adds player data to entity and handles palette changes
 */
class PlayerComponent {
CLASS_COMPONENT(Entity, PlayerComponent)
protected:
    /**
     * Player for this entity
     */
    Player* player = nullptr;

public:
    /**
     * @return this entity player if any
     */
    Player* getPlayer();

     /**
      * Set this entity player
      */
    void setPlayer(Player* newPlayer);
};

#endif //OPENE2140_PLAYER_COMPONENT_H
