//
// Created by Ion Agorria on 1/11/18
//
#ifndef OPENE2140_GAME_H
#define OPENE2140_GAME_H

#include "common.h"
#include "graphics/renderer.h"
#include "simulation/simulation.h"
#include "graphics/window.h"
#include "assets/assetmanager.h"
#include "gui/eventhandler.h"
#include "luavm.h"

/**
 * Contains the central game code that calls and coordinates the subsystems
 */
class Game: public std::enable_shared_from_this<Game> {
    /**
     * Log for game
     */
    log_ptr log;

    /**
     * Manager for asset fetching
     */
    std::unique_ptr<AssetManager> assetManager;

    /**
     * Window where the game gets drawn
     */
    std::unique_ptr<Window> window;

    /**
     * Stores the game simulation state
     */
    std::unique_ptr<Simulation> simulation;

    /**
     * Event handler to receive window events
     */
    std::unique_ptr<EventHandler> eventHandler;

    /**
     * Renderer which handles the drawing of game
     */
    std::unique_ptr<Renderer> renderer;

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
     * Game close method
     */
    void close();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Game)

    /**
     * Get shared pointer to this game
     *
     * @return shared game pointer
     */
    std::shared_ptr<Game> this_shared_ptr() {
        return shared_from_this();
    }

    /**
     * Initializes game
     *
     * @return true if success
     */
    bool run();

    void loop();
};

#endif //OPENE2140_GAME_H
