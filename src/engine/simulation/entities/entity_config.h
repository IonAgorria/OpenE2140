//
// Created by Ion Agorria on 22/06/19
//
#ifndef OPENE2140_ENTITY_CONFIG_H
#define OPENE2140_ENTITY_CONFIG_H

#include <unordered_map>
#include "engine/core/types.h"
#include "engine/io/config.h"

/**
 * Base entity config containing the entity stats, type and such data
 */
class EntityConfig: public entity_type_t, public config_data_t {
public:
    EntityConfig() = default;
    ~EntityConfig() = default;
};

#endif //OPENE2140_ENTITY_CONFIG_H
