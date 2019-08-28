//
// Created by Ion Agorria on 28/04/19
//

#include "game/core/game.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/world/world.h"
#include "event_listener_debug.h"

EventListenerDebug::EventListenerDebug(const std::shared_ptr<Game>& game): game(game) {
    keyDebugEntities = game->getKeyBind("F2");
    keyDebugTiles = game->getKeyBind("F3");
}

EventListenerDebug::~EventListenerDebug() {
    if (game) {
        game.reset();
    }
}

bool EventListenerDebug::eventKeyChange(Window* window, input_key_t& key) {
    if (key.press && !key.repeat) {
        if (key.code == keyDebugTiles) {
            Simulation* simulation = game->getSimulation();
            simulation->getWorld()->debugTiles = !simulation->getWorld()->debugTiles;
        } else if (key.code == keyDebugEntities) {
            Simulation* simulation = game->getSimulation();
            simulation->debugEntities = !simulation->debugEntities;
        }
    }
    return false;
}