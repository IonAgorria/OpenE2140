//
// Created by Ion Agorria on 15/8/19
//
#ifndef OPENE2140_FACTORIES_H
#define OPENE2140_FACTORIES_H

#include "engine/io/config.h"
#include "game/core/constants.h"
#include "src/engine/entities/entity_factory.h"
#include "building.h"
#include "object.h"
#include "unit.h"
#include "attachment.h"

/**
 * Abstract common factory
 */
class ACommonEntityFactory: public IEntityFactory {
public:
    void setupEntityConfig(EntityConfig* config) override {
        //Set config values if they are missing
        config->setData("attachments_kind", config_data_t(ENTITY_KIND_ATTACHMENT));

        IEntityFactory::setupEntityConfig(config);
    }
};

/**
 * Object factory
 */
class ObjectFactory: public ACommonEntityFactory {
    TYPE_NAME_OVERRIDE(ObjectFactory);

    std::string getConfigPath() const override {
        return "objects.json";
    }

    std::string getAssetPath() const override {
        return "MIX/SPRO";
    }

    std::vector<std::string> getVariants() const override {
        return ENTITY_OBJECTS_VARIANTS;
    }

    entity_kind_t getKind() const override {
        return ENTITY_KIND_OBJECT;
    }

    std::shared_ptr<Entity> instanceEntity(EntityConfig* config) override {
        if (config) {
            if (config->type == "tree") {
                return std::make_shared<Tree>();
            }
        }
        return std::make_shared<Object>();
    }

    void setupEntityConfig(EntityConfig* config) override {
        //Set config values if they are missing
        if (!config->getData("palette_shadow").is_boolean()
        && (config->type == "wall" || config->type == "pipe")) {
            config->setData("palette_shadow", true);
        }

        //Set palette size using the entity config data
        if (config->getData("palette_shadow", false)) {
            config->setData("palette_lowest_entry", PALETTE_SHADOW);
        }

        ACommonEntityFactory::setupEntityConfig(config);
    }
};

/**
 * Unit factory
 */
class UnitFactory: public ACommonEntityFactory {
    TYPE_NAME_OVERRIDE(UnitFactory);

    std::string getConfigPath() const override {
        return "units.json";
    }

    std::string getAssetPath() const override {
        return "MIX/SPRU0/";
    }

    entity_kind_t getKind() const override {
        return ENTITY_KIND_UNIT;
    }

    std::shared_ptr<Entity> instanceEntity(EntityConfig* config) override {
        return std::make_shared<Unit>();
    }

    void setupEntityConfig(EntityConfig* config) override {
        //Set config values if they are missing
        if (!config->getData("palette_movement").is_boolean()
            && (config->type == "mcu" || config->type == "vehicle" || config->type == "tank")) {
            config->setData("palette_movement", true);
        }
        if (!config->getData("palette_player").is_boolean()) {
            config->setData("palette_player", true);
        }
        if (!config->getData("palette_shadow").is_boolean()) {
            config->setData("palette_shadow", true);
        }

        //Set palette size using the entity config data
        if (config->getData("palette_movement", false)) {
            config->setData("palette_lowest_entry", PALETTE_UNIT_MOVEMENT0);
        } else if (config->getData("palette_fire", false)) {
            config->setData("palette_lowest_entry", PALETTE_UNIT_FIRE0);
        } else if (config->getData("palette_light", false)) {
            config->setData("palette_lowest_entry", PALETTE_UNIT_LIGHT);
        } else if (config->getData("palette_player", false)) {
            config->setData("palette_lowest_entry", PALETTE_PLAYER);
        } else if (config->getData("palette_shadow", false)) {
            config->setData("palette_lowest_entry", PALETTE_SHADOW);
        }

        ACommonEntityFactory::setupEntityConfig(config);
    }
};

/**
 * Building factory
 */
class BuildingFactory: public ACommonEntityFactory {
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

    std::shared_ptr<Entity> instanceEntity(EntityConfig* config) override {
        if (config) {
            if (config->type == "construction_factory"
            || config->type == "light_factory"
            || config->type == "heavy_factory"
            || config->type == "heavy_factory") {
                return std::make_shared<Factory>();
            }
        }
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

        //Set config values if they are missing
        if (!config->getData("palette_light").is_boolean()) {
            config->setData("palette_light", true);
        }
        if (!config->getData("palette_player").is_boolean()) {
            config->setData("palette_player", true);
        }
        if (!config->getData("palette_shadow").is_boolean()) {
            config->setData("palette_shadow", true);
        }

        //Set palette size using the entity config data
        if (config->getData("palette_light", false)) {
            config->setData("palette_lowest_entry", PALETTE_BUILDING_LIGHT0);
        } else if (config->getData("palette_player", false)) {
            config->setData("palette_lowest_entry", PALETTE_PLAYER);
        } else if (config->getData("palette_shadow", false)) {
            config->setData("palette_lowest_entry", PALETTE_BUILDING_SHADOW_EXTRA);
        }

        ACommonEntityFactory::setupEntityConfig(config);
    }
};

/**
 * Attachment factory
 */
class AttachmentFactory: public ACommonEntityFactory {
    TYPE_NAME_OVERRIDE(AttachmentFactory);

    std::string getConfigPath() const override {
        return "attachments.json";
    }

    entity_kind_t getKind() const override {
        return ENTITY_KIND_ATTACHMENT;
    }

    std::shared_ptr<Entity> instanceEntity(EntityConfig* config) override {
        if (config) {
            if (config->type == "attachment_spinner") {
                return std::make_shared<Spinner>();
            }
        }
        return std::make_shared<Attachment>();
    }
};

#endif //OPENE2140_FACTORIES_H
