//
// Created by Ion Agorria on 12/07/19
//
#ifndef OPENE2140_FACTION_COMPONENT_H
#define OPENE2140_FACTION_COMPONENT_H

#include "engine/simulation/components/component.h"

class Entity;
class Faction;

/**
 * Adds faction information if available from config
 */
CLASS_COMPONENT(Entity, FactionComponent)
protected:
    /**
     * Faction for this entity
     */
    Faction* faction;

public:
    /**
     * @return this entity faction if any
     */
    Faction* getFaction();

     /**
      * Set this entity faction
      */
    void setFaction(Faction* newFaction);
};

#endif //OPENE2140_FACTION_COMPONENT_H
