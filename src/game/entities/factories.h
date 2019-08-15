//
// Created by Ion Agorria on 15/8/19
//
#ifndef OPENE2140_FACTORIES_H
#define OPENE2140_FACTORIES_H

#include <src/engine/io/config.h>
#include "game/core/constants.h"
#include "engine/simulation/entities/entity_factory.h"
#include "building.h"
#include "object.h"

/**
 * Building factory
 */
class BuildingFactory: public IEntityFactory {
    TYPE_NAME_OVERRIDE(BuildingFactory);

    std::string getConfigPath() const override {
        return "buildings.json";
    }

    std::string getAssetPath() const override {
        return "MIX/SPRB0/";
    }

    entity_kind_t getKind() const override {
        return ENTITY_KIND_BUILDING;
    }

    std::shared_ptr<Entity> instanceEntity(entity_type_id_t id, EntityConfig* config) override {
        return std::make_shared<Building>();
    }

    void setupEntityConfig(EntityConfig* config) override {
        //Setup the bounds from first layout of building if bounds is not set in config and layout exists
        config_data_t layout = config->getData("layout");
        if (!config->getData("bounds").is_array() && layout.is_array()) {
            Rectangle bounds;
            if (Config::getRectangle(layout[0], bounds)) {
                config->setData("bounds", layout[0]);
                config->bounds = bounds;
            }
        }

        IEntityFactory::setupEntityConfig(config);
    }
};

/**
 * Object factory
 */
class ObjectFactory: public IEntityFactory {
    TYPE_NAME_OVERRIDE(ObjectFactory);

    std::string getConfigPath() const override {
        return "objects.json";
    }

    std::string getAssetPath() const override {
        return "MIX/SPRO";
    }

    std::vector<std::string> getVariants() const override {
        return {"0", "1", "2", "3", "4", "5", "6"};
    }

    asset_path_t assembleAssetPath(const asset_path_t& path, const std::string& variant, const std::string& index) const override {
        //Use special variant handling
        return (path.empty() ? (getAssetPath() + variant + "/") : path) + index;
    }

    entity_kind_t getKind() const override {
        return ENTITY_KIND_OBJECT;
    }

    std::shared_ptr<Entity> instanceEntity(entity_type_id_t id, EntityConfig* config) override {
        if (config) {
            if (config->type == "tree") {
                return std::make_shared<Tree>();
            }
        }
        return std::make_shared<Object>();
    }
};

#endif //OPENE2140_FACTORIES_H
