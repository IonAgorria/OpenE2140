//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_PLAYER_SETUP_COMPONENT_H
#define OPENE2140_PLAYER_SETUP_COMPONENT_H

#include "game/core/constants.h"
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
#define PALETTE_UNIT_LIGHT 0xF7
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
 * Handles palette setup per entity type
 */
class PaletteComponent {
CLASS_COMPONENT_BODY(Entity, PaletteComponent)
private:
    /**
     * Palette controlled by this component
     */
    std::unique_ptr<Palette> palette;

    /**
     * Lowest entry for palette index conversion
     */
    size_t lowestEntry = 0;

public:
    /**
     * Destructor
     */
    ~PaletteComponent() = default;

    void construction();

    void setup();

    void simulationChanged();

    void update();

    void draw();

    /**
     * Does setup for shadow colors
     * @param config
     */
    void setupShadows(const EntityConfig* config);

    /**
     * @return extra palette in component if any
     */
    Palette* getPalette() const;
};

#endif //OPENE2140_PLAYER_SETUP_COMPONENT_H
