//
// Created by Ion Agorria on 27/04/19
//
#ifndef OPENE2140_GAME_H
#define OPENE2140_GAME_H

#include "engine/core/engine.h"

/**
 * Contain's game specific code
 */
class Game: public Engine {
protected:
    void run() override;

    void setupEventHandler() override;

    void setupAssetManager() override;
};

#endif //OPENE2140_GAME_H
