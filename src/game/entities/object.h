//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_OBJECT_H
#define OPENE2140_OBJECT_H

#include "game/core/constants.h"
#include "engine/simulation/entities/entity_factory.h"
#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/entities/entity.h"

/**
 * Simple object with simple image
 */
CLASS_ENTITY_COMPONENTS(Entity, Object, ImageComponent)
public:
    void simulationChanged() override;

    void draw() override;
};

/**
 * Tree object
 */
CLASS_ENTITY(Object, Tree)
public:
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

#endif //OPENE2140_OBJECT_H
