//
// Created by Ion Agorria on 29/06/19
//

#include "entity.h"
#include "engine/assets/asset_manager.h"
#include "entity_manager.h"
#include "entity_factory.h"
#include "engine/io/config.h"
#include "engine/core/utils.h"


void IEntityFactory::clear() {
}

void IEntityFactory::load() {
    loadConfig(Utils::getDataPath() + getConfigPath());
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
    entity_type_id_t id = data.value("id", 0);
    //Allocate new empty if not enough are present
    if (configs.size() < id + 1) {
        configs.resize(id + 1);
    }
    //Create config and set it in configs
    std::unique_ptr<EntityConfig> config = std::make_unique<EntityConfig>(data);
    config->kind = getKind();
    config->id = id;
    configs[id].swap(config);
}

void IEntityFactory::setManager(EntityManager* current) {
    manager = current;
}

std::shared_ptr<Entity> IEntityFactory::makeEntity(entity_type_id_t id) {
    //Get the config for entity instance
    EntityConfig* config = nullptr;
    if (id < configs.size()) {
        config = configs[id].get();
    }
    //Call the implementation
    std::shared_ptr<Entity> entity = instanceEntity(id, config);
    //Only setup if entity was instanced
    if (!config) {
        error = "Entity config not found for id: " + std::to_string(id);
    } else if (!entity) {
        error = "Entity not created for id: " + std::to_string(id);
    } else {
        entity->setup(config);
    }
    return entity;
}

Image* IEntityFactory::getImage(const asset_path_t& path) const {
    return manager->getAssetManager()->getImage(path).get();
}
