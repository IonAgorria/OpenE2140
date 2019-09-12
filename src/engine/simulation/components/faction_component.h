//
// Created by Ion Agorria on 12/07/19
//
#ifndef OPENE2140_FACTION_COMPONENT_H
#define OPENE2140_FACTION_COMPONENT_H

#include "component.h"

class Entity;
class Faction;

/**
 * Adds faction information if available from config
 */
class FactionComponent {
CLASS_COMPONENT(Entity, FactionComponent)
protected:
    /**
     * Faction for this entity
     */
    Faction* faction = nullptr;

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
