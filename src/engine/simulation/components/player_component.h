//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_PLAYER_COMPONENT_H
#define OPENE2140_PLAYER_COMPONENT_H

#include "src/engine/simulation/components/component.h"

class Entity;
class Player;

/**
 * Adds player data to entity and handles palette changes
 */
CLASS_COMPONENT(Entity, PlayerComponent)
protected:

public:
    /**
     * Player for this entity
     */
    Player* player;

    /**
     *
     */
};

#endif //OPENE2140_PLAYER_COMPONENT_H
