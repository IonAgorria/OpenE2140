//
// Created by Ion Agorria on 12/07/19
//
#ifndef OPENE2140_FACTION_H
#define OPENE2140_FACTION_H

#include "engine/core/common.h"
#include "engine/io/has_config_data.h"

/**
 * Contains faction related data
 */
class Faction: public IHasConfigData {
public:
    /**
     * ID for the faction
     */
    faction_id_t id;

    /**
     * Code for this faction
     */
    std::string code;

    /**
     * Name for this faction
     */
    std::string name;
};

#endif //OPENE2140_FACTION_H
