//
// Created by Ion Agorria on 28/8/19
//

#include "engine/core/macros.h"
#include "engine/io/log.h"
#include "faction.h"

Faction::Faction(faction_id_t id): id(id) {
    if (id == 0) BUG("Faction ID is 0");
}
