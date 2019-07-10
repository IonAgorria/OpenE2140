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
    if (!config.data.is_array()) {
        error = "Config doesn't contain array as root";
        return;
    }

    //Load each entity config
    for (nlohmann::json& data : config.data) {
        loadEntityConfig(data);
    }
}

void IEntityFactory::loadEntityConfig(config_data_t& data) {
    //Get ID
    if (!data["id"].is_number_integer()) return;
    entity_type_id_t id = data["id"].get<entity_type_id_t>();
    //Allocate new empty if not enough configs are present
    if (configs.size() < id + 1) {
        configs.resize(id + 1);
    }
    //Create config and set it in configs
    std::unique_ptr<EntityConfig> config = std::make_unique<EntityConfig>();
    config->kind = getKind();
    config->id = id;
    config->update(data);
    configs[id].swap(config);
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