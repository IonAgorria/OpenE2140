//
// Created by Ion Agorria on 1/11/18
//
#ifndef OPENE2140_ENGINE_H
#define OPENE2140_ENGINE_H

#include <memory>
#include "engine/core/common.h"
#include "engine/core/error_possible.h"
#include "io/log.h"

class AssetManager;
class Window;
class EventHandler;
class Renderer;
class Simulation;
class Timer;

/**
 * Contains the central game code that calls and coordinates the subsystems
 */
class Engine: public std::enable_shared_from_this<Engine>, public IErrorPossible {
protected:
    /**
     * Log for game
     */
    log_ptr log;

    /**
     * Event handler to receive window events
     */
    std::unique_ptr<EventHandler> eventHandler;

    /**
     * Main window
     */
    std::unique_ptr<Window> window;

    /**
     * Renderer which handles the drawing of game
     */
    std::unique_ptr<Renderer> renderer;

    /**
     * Manager for asset fetching
     */
    std::unique_ptr<AssetManager> assetManager;

    /**
     * Stores the simulation state
     */
    std::unique_ptr<Simulation> simulation;

    /**
     * Stores the engine timer
     */
    std::unique_ptr<Timer> timer;

    /**
     * Called when engine is requested to close
     */
    virtual void close();

    /**
     * Initializes engine
     */
    virtual void run();

    /**
     * Updates engine data
     */
    void update();

    /**
     * Draws engine data
     */
    void draw();

    /**
     * Called from engine to setup EventHandler
     */
    virtual void setupEventHandler();

    /**
     * Called from engine to setup Window
     */
    virtual void setupWindow();

    /**
     * Called from engine to setup Renderer
     */
    virtual void setupRenderer();

    /**
     * Called from engine to setup AssetManager
     */
    virtual void setupAssetManager();

    /**
     * Called from engine to setup Simulation
     */
    virtual void setupSimulation();
public:
    /**
     * Main engine entry point, does the basic initializations
     *
     * @param argc number of args
     * @param argv args array
     * @return program exit code
     */
    static int main(int argc, char** argv, std::shared_ptr<Engine> engine);

    /**
     * Constructor
     */
    Engine();

    /**
     * Destructor
     */
    ~Engine();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Engine)

    /**
     * Get shared pointer to this engine
     *
     * @return shared pointer
     */
    template<typename T>
    std::shared_ptr<T> this_shared_ptr() {
        return std::dynamic_pointer_cast<T>(shared_from_this());
    }

    /**
     * @return event handler
     */
    EventHandler* getEventHandler();

    /**
     * @return main Window
     */
    Window* getWindow();

    /**
     * @return Renderer
     */
    Renderer* getRenderer();

    /**
     * @return AssetManager
     */
    AssetManager* getAssetManager();

    /**
     * @return Simulation
     */
    Simulation* getSimulation();
};

#endif //OPENE2140_ENGINE_H
