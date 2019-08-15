//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_BUILDING_H
#define OPENE2140_BUILDING_H

#include "engine/simulation/entities/entity_factory.h"
#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/components/faction_component.h"
#include "engine/simulation/components/player_component.h"
#include "game/components/palette_setup_component.h"
#include "game/core/constants.h"
#include "engine/simulation/entities/entity.h"

class Building;
CLASS_COMPONENT(Building, BuildingComponent)
};

/**
 * Building entity
 */
CLASS_ENTITY_COMPONENTS(Entity, Building,
                        PlayerComponent,
                        FactionComponent,
                        PaletteSetupComponent, //before ImageComponent
                        ImageComponent
)
public:
    void simulationChanged() override;

    void draw() override;

    /**
     * Selects current sprite
     */
    void chooseSprite();
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

    void setupEntityConfig(EntityConfig* config) override;
};

#endif //OPENE2140_BUILDING_H
