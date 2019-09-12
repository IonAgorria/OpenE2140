//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_PALETTE_COMPONENT_H
#define OPENE2140_PALETTE_COMPONENT_H

#include "game/core/constants.h"
#include "engine/graphics/color.h"
#include "engine/simulation/components/component.h"

/*
 * Unit entries
 */
//Unit movement dependent colors (threads/wheels/jets)
#define PALETTE_UNIT_MOVEMENT0 0xF0
#define PALETTE_UNIT_MOVEMENT1 0xF1
#define PALETTE_UNIT_MOVEMENT2 0xF2
#define PALETTE_UNIT_MOVEMENT3 0xF3
//Unit weapon fire
#define PALETTE_UNIT_FIRE0 0xF4
#define PALETTE_UNIT_FIRE1 0xF5
#define PALETTE_UNIT_FIRE2 0xF6
//Unit light
#define PALETTE_UNIT_LIGHT 0xF7
//TODO
#define PALETTE_UNIT_UNKNOWN 0xFD

/*
 * Building entries
 */
//Building lights to be set as black when has no power (water base doesn't have any so not set)
#define PALETTE_BUILDING_LIGHT0 0xF4
#define PALETTE_BUILDING_LIGHT1 0xF5
#define PALETTE_BUILDING_LIGHT2 0xF6
#define PALETTE_BUILDING_LIGHT3 0xF7
//Building shadow tones
#define PALETTE_BUILDING_SHADOW_EXTRA 0xFD

/**
 * Color definitions
 */
namespace Color {
    const ColorRGBA BUILDING_LIGHT0_ON {{25, 25, 185, 255}};
    const ColorRGBA BUILDING_LIGHT1_ON {{45, 45, 205, 255}};
    const ColorRGBA BUILDING_LIGHT2_ON {{65, 65, 225, 255}};
    const ColorRGBA BUILDING_LIGHT3_ON {{85, 85, 245, 255}};
    const ColorRGBA BUILDING_LIGHT_OFF {{0, 0, 10, 255}};
}

/**
 * Handles palette setup per entity type
 */
class PaletteComponent {
CLASS_COMPONENT(Entity, PaletteComponent)
private:
    /**
     * Palette controlled by this component
     */
    std::shared_ptr<Palette> palette;

    /**
     * Lowest entry for palette index conversion
     */
    size_t lowestEntry = 0;

    /**
     * Does this entity have player defined colors?
     */
    bool hasPlayer = false;

    /**
     * Does this entity have lights?
     */
    bool hasLight = false;

    /**
     * Does this entity have shadow?
     */
    bool hasShadow = false;

    /**
     * Is the shadow clear?
     */
    bool isShadowClear = false;

    /**
     * Does this entity have movement based colors?
     */
    bool hasMovement = false;

    /**
     * Does this entity have weapon fire?
     */
    bool hasFire = false;

public:
    /**
     * Does setup for shadow colors
     * @param config
     */
    void setupShadows(const EntityConfig* config);

    /**
     * Controls light state
     *
     * @param state
     */
    void setLight(bool state);

    /**
     * @return palette
     */
    std::shared_ptr<Palette> getPalette();
};

#endif //OPENE2140_PALETTE_COMPONENT_H
