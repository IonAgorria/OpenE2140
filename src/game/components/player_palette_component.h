//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_PLAYER_PALETTE_COMPONENT_H
#define OPENE2140_PLAYER_PALETTE_COMPONENT_H

#include "engine/simulation/components/component.h"

class Building;
class Player;

/**
 * Handles player dependent palettes
 */
template<typename Base>
class PlayerPaletteComponent {
CLASS_COMPONENT_BODY(Base, PlayerPaletteComponent)
public:
    /**
     * Destructor
     */
    ~PlayerPaletteComponent() = default;

    void construction() {}

    void setup() {}

    void simulationChanged() {
        //Load player color
        Player* player = base->getPlayer();
        if (player) {
            //TODO
        }
    }

    void update() {}

    void draw() {}
};

#endif //OPENE2140_PLAYER_PALETTE_COMPONENT_H
