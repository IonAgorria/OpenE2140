//
// Created by Ion Agorria on 1/11/18
//
#ifndef OPENE2140_ENGINE_H
#define OPENE2140_ENGINE_H

#include "engine/core/macros.h"
#include "engine/simulation/simulation_parameters.h"
#include "engine/core/error_possible.h"
#include "engine/io/log.h"
#include "engine/io/has_config_data.h"
#include "engine/math/vector2.h"

//Forward declarations
class AssetManager;
class EntityManager;
class Window;
class EventHandler;
class Renderer;
class Simulation;
class Timer;
class GUIRoot;
class Locale;
class Entity;

/**
 * Contains the central game code that calls and coordinates the subsystems
 */
class Engine: public std::enable_shared_from_this<Engine>, public IErrorPossible, public IHasConfigData {
protected:
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
     * Manager for entity management
     */
    std::unique_ptr<EntityManager> entityManager;

    /**
     * Stores the simulation state
     */
    std::unique_ptr<Simulation> simulation;

    /**
     * Stores the engine update timer
     */
    std::unique_ptr<Timer> updateTimer;

    /**
     * Last read update timer elapsed value
     */
    float updateTimerElapsed = 0;

    /**
     * Stores the engine draw timer
     */
    std::unique_ptr<Timer> drawTimer;

    /**
     * Current active menu if any
     */
    std::shared_ptr<GUIRoot> guiRoot;

    /**
     * Available locale and their name
     */
    std::unordered_map<std::string, std::string> locales;

    /**
     * Current locale
     */
    std::unique_ptr<Locale> locale;

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
     * Called from engine to setup EntityManager
     */
    virtual void setupEntityManager();

    /**
     * Called from engine to setup GUI elements
     */
    virtual void setupGUI();

    /**
     * Should be called to setup Simulation
     *
     * @param parameters
     */
    virtual void setupSimulation(std::unique_ptr<SimulationParameters> parameters);

    /**
     * Called from engine to load engine config
     */
    virtual void loadConfig();

    /**
     * Called from engine to save engine config
     */
    virtual void saveConfig();

    /**
     * Called from engine to load locales and setup the current locale
     */
    virtual void setupLocale();

    /**
     * Sets the locale as current
     *
     * @param code locale code to set as current
     * @param persistent sets the locale in the config as current
     */
    virtual void setLocale(const std::string& code, bool persistent);

    /**
     * Sets the GUI root as current
     *
     * @param code locale code to set as current
     */
    virtual void setGUI(std::shared_ptr<GUIRoot> root);

    /**
     * Loads factions from data
     */
    virtual void loadFactions();

public:
    /**
     * Log for engine
     */
    log_ptr log;

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
    ~Engine() override;

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
     * @return EntityManager
     */
    EntityManager* getEntityManager();

    /**
     * @return Simulation
     */
    Simulation* getSimulation();

    /**
     * @return key code for provided bind
     */
    input_key_code_t getKeyBind(const std::string& name);

    /**
     * @return translated text for provided text key
     */
    const std::string& getText(const std::string& key);
};

#endif //OPENE2140_ENGINE_H
