//
// Created by Ion Agorria on 29/06/19
//

#include "entity.h"
#include "entity_factory.h"
#include "engine/io/config.h"
#include "engine/core/utils.h"


void IEntityFactory::clear() {
}

void IEntityFactory::load() {
    //Load roots
    std::vector<std::string> roots;
    Utils::getRootPaths(getConfigPath(), roots);
    bool loaded = false;
    for (const std::string& path : roots) {
        loadConfig(path);
        if (hasError()) {
            continue;
        }
        loaded = true;
        break;
    }

    if (!loaded) {
        error = error + "\nCouldn't load any config";
    }
}

void IEntityFactory::loadConfig(const std::string& path) {
    //Load config
    Config config(path);
    config.read();
    error = config.getError();
    if (hasError()) {
        return;
    }

    //Check if type is correct
    if (config.data.type() != nlohmann::detail::value_t::array) {
        error = "Config doesn't contain array as root";
        return;
    }


}

void IEntityFactory::loadEntityConfig(config_data_t& data) {
}

void IEntityFactory::setManager(EntityManager* current) {
    manager = current;
}

std::shared_ptr<Entity> IEntityFactory::makeEntity(entity_type_id_t id) {
    std::shared_ptr<Entity> entity = instanceEntity(id);
    //Only setup if entity was instanced
    if (entity) {
        EntityConfig* config = nullptr;
        if (id < configs.size()) {
            config = configs[id].get();
        }
        if (config) {
            entity->setup(config);
        } else {
            error = "Entity config not found for id: " + std::to_string(id);
        }
    }
    return entity;
}