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
CLASS_ENTITY(Entity, Object, ImageComponent)
public:
    Object();

    void draw() override;
};

/**
 * Tree object
 */
CLASS_ENTITY(Entity, Tree, ImageComponent)
public:
    Tree();

    void draw() override;
};

/**
 * Object factory
 */
class ObjectFactory: public IEntityFactory {
    TYPE_NAME_OVERRIDE(ObjectFactory);

    std::string getConfigPath() override {
        return "objects.json";
    }

    entity_kind_t getKind() override {
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
