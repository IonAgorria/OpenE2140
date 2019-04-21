//
// Created by Ion Agorria on 1/11/18
//
#ifndef OPENE2140_GAME_H
#define OPENE2140_GAME_H

#include <memory>
#include "common.h"
#include "io/log.h"

/**
 * Contains the central game code that calls and coordinates the subsystems
 */
class AssetManager;
class Window;
class EventHandler;
class Renderer;
class Simulation;
class LuaVM;
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
     * Main window pointer
     */
    Window* mainWindow;

    /**
     * Windows where the game gets drawn
     */
    std::unordered_map<unsigned int, std::unique_ptr<Window>> windows;

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

    /**
     * Main loop for game
     */
    void loop();

    /**
     * @return main window
     */
    Window* getMainWindow();

    /**
     * @param id of window to get
     * @return Window
     */
    Window* getWindow(unsigned int id);

    /** @return Renderer */
    Renderer* getRenderer();

    /** @return Simulation */
    Simulation* getSimulation();

    /** @return AssetManager */
    AssetManager* getAssetManager();

    void test(int i);
};

#endif //OPENE2140_GAME_H
