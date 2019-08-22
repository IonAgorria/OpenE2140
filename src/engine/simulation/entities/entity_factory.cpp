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
    log = Log::get(type_name());
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
        //Get ID
        if (!data["id"].is_number_integer()) return;
        entity_type_id_t id = data.value("id", 0);
        //Allocate new empty if not enough are present
        if (configs.size() < id + 1) {
            configs.resize(id + 1);
        }
        //Create config and set it in configs
        std::unique_ptr<EntityConfig> entityConfig = std::make_unique<EntityConfig>();
        entityConfig->kind = getKind();
        entityConfig->id = id;
        entityConfig->loadEntityData(data, this);
        setupEntityConfig(entityConfig.get());
        configCodes[entityConfig->code] = id;
        configs[id].swap(entityConfig);
    }
}

void IEntityFactory::setupEntityConfig(EntityConfig* config) {
}

std::string IEntityFactory::getAssetPath() const {
    return "";
}

std::vector<std::string> IEntityFactory::getVariants() const {
    return {""};
}

asset_path_t IEntityFactory::assembleAssetPath(const asset_path_t& path, const std::string& variant, const std::string& index) const {
    return (path.empty() ? getAssetPath() : path) + (variant.empty() ? "" : variant + "/") + index;
}

std::string IEntityFactory::assembleGroupCode(const std::string& name, const std::string& variant, const std::string& collection) const {
    return name
    + (variant.empty() ? "" : "_" + variant)
    + (collection.empty() ? "" : "_" + collection)
    ;
}

void IEntityFactory::setManager(EntityManager* current) {
    manager = current;
}

std::shared_ptr<Entity> IEntityFactory::makeEntity(const entity_type_t& type) {
    std::shared_ptr<Entity> entity;
    //Get entity type id
    entity_type_id_t id;
    if (type.code.empty()) {
        id = type.id;
    } else {
        auto it = configCodes.find(type.code);
        if (it == configCodes.end()) {
            //Not found
            error = "Entity config not found for code: " + type.code;
            return entity;
        } else {
            id = it->second;
        }
    }
    //Get the config for entity instance
    EntityConfig* config = nullptr;
    if (id < configs.size()) {
        config = configs[id].get();
    }
    //Call the implementation
    entity = instanceEntity(id, config);
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

log_ptr IEntityFactory::getLog() const {
    return log;
}

Image* IEntityFactory::getImage(const asset_path_t& path) const {
    return manager->getAssetManager()->getImage(path).get();
}
