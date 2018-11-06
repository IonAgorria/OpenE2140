//
// Created by Ion Agorria on 1/11/18
//
#ifndef OPENE2140_GAME_H
#define OPENE2140_GAME_H


#include "graphics/window.h"
#include "assets/manager.h"
#include "gui/eventhandler.h"
#include "luavm.h"

/**
 * Contains the central game code that calls and coordinates the subsystems
 */
class Game {
    /**
     * Log for game
     */
    log_ptr log;

    /**
     * Manager for asset fetching
     */
    std::unique_ptr<Manager> assetManager;

    /**
     * Window where the game gets drawn
     */
    std::unique_ptr<Window> window;

    /**
     * Lua VM for game
     */
    std::shared_ptr<LuaVM> luaVM;

public:
    /**
     * Game constructor
     */
    Game();

    /**
     * Game destructor
     */
    ~Game();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Game)

    /**
     * Initializes game
     *
     * @return true if success
     */
    bool run();
};

#endif //OPENE2140_GAME_H
