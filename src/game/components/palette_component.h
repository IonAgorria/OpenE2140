//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_PLAYER_SETUP_COMPONENT_H
#define OPENE2140_PLAYER_SETUP_COMPONENT_H

#include "game/core/constants.h"
#include "engine/graphics/color.h"
#include "engine/simulation/components/component.h"

/**
 * Constants for palette indexes with special meaning
 */
#define PALETTE_MAX_INDEX 0xFF
//Object shadow
#define PALETTE_OBJECT_SHADOW 0xFE
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
#define PALETTE_UNIT_LIGHT0 0xF7
//Unit player coloring
#define PALETTE_UNIT_PLAYER0 0xF8
#define PALETTE_UNIT_PLAYER1 0xF9
#define PALETTE_UNIT_PLAYER2 0xFA
#define PALETTE_UNIT_PLAYER3 0xFB
#define PALETTE_UNIT_PLAYER4 0xFC
#define PALETTE_UNIT_PLAYER5 0xFD
//Unit shadow
#define PALETTE_UNIT_SHADOW 0xFE

//Building lights to be set as black when has no power (water base doesn't have any so not set)
#define PALETTE_BUILDING_LIGHT0 0xF4
#define PALETTE_BUILDING_LIGHT1 0xF5
#define PALETTE_BUILDING_LIGHT2 0xF6
#define PALETTE_BUILDING_LIGHT3 0xF7
//Building player coloring
#define PALETTE_BUILDING_PLAYER0 0xF8
#define PALETTE_BUILDING_PLAYER1 0xF9
#define PALETTE_BUILDING_PLAYER2 0xFA
#define PALETTE_BUILDING_PLAYER3 0xFB
#define PALETTE_BUILDING_PLAYER4 0xFC
//Building shadow tones
#define PALETTE_BUILDING_SHADOW_EXTRA 0xFD
#define PALETTE_BUILDING_SHADOW 0xFE

/**
 * Color definitions
 */
namespace Color {
    const ColorRGBA BUILDING_LIGHT0_ON {25, 25, 185, 255};
    const ColorRGBA BUILDING_LIGHT1_ON {45, 45, 205, 255};
    const ColorRGBA BUILDING_LIGHT2_ON {65, 65, 225, 255};
    const ColorRGBA BUILDING_LIGHT3_ON {85, 85, 245, 255};
    const ColorRGBA BUILDING_LIGHT_OFF {0, 0, 10, 255};
    const ColorRGBA BUILDING_PLAYER0 {0, 30, 0, 255};
    const ColorRGBA BUILDING_PLAYER1 {20, 50, 0, 255};
    const ColorRGBA BUILDING_PLAYER2 {40, 60, 0, 255};
    const ColorRGBA BUILDING_PLAYER3 {45, 75, 0, 255};
    const ColorRGBA BUILDING_PLAYER4 {55, 90, 0, 255};
}

/**
 * Handles palette setup per entity type
 */
class PaletteComponent {
CLASS_COMPONENT_BODY(Entity, PaletteComponent)
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
     * Does this entity have lights?
     */
    bool hasLight = false;

public:
    /**
     * Destructor
     */
    ~PaletteComponent() = default;

    void construction();

    void setup();

    void simulationChanged();

    void update();

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
     * @return extra palette in component if any
     */
    Palette* getPalette() const;
};

#endif //OPENE2140_PLAYER_SETUP_COMPONENT_H
