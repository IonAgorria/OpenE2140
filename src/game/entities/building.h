//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_BUILDING_H
#define OPENE2140_BUILDING_H

#include "engine/simulation/entities/entity_factory.h"
#include "engine/simulation/components/component.h"
#include "engine/simulation/components/faction_component.h"
#include "engine/simulation/components/player_component.h"
#include "game/components/player_palette_component.h"
#include "game/core/constants.h"
#include "engine/simulation/entities/entity.h"

/**
 * Building entity
 */
CLASS_ENTITY(Entity, Building,
        PlayerComponent,
        FactionComponent,
        PlayerPaletteComponent<Building>
)
public:
    Building();

    void draw() override;
};

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
};

#endif //OPENE2140_BUILDING_H
