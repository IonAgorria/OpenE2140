//
// Created by Ion Agorria on 1/11/18
//
#include <SDL.h>
#include "engine/assets/asset_manager.h"
#include "engine/graphics/palette.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/window.h"
#include "engine/gui/gui_root.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/faction.h"
#include "engine/simulation/world/world.h"
#include "src/engine/entities/entity_manager.h"
#include "engine/io/event_handler.h"
#include "engine/core/utils.h"
#include "engine/io/timer.h"
#include "engine/io/config.h"
#include "engine/gui/locale.h"
#include "engine.h"

int Engine::main(int argc, char** argv, std::shared_ptr<Engine> engine) {
    //Register signal and terminate handler
    Utils::setSignalHandler(Utils::handleHaltAndCatchFire, Utils::handleTerminate);

    //Parse args
    for(int i=1; i < argc; i++) {
        std::string arg = argv[i];
        std::transform(BEGIN_END(arg), arg.begin(), ::tolower);
        if (arg == "--debug" || arg == "-d") {
            Utils::setFlag(FLAG_DEBUG, true);
        } else if (arg == "--debug_all" || arg == "-da") {
            Utils::setFlag(FLAG_DEBUG, true);
            Utils::setFlag(FLAG_DEBUG_ALL, true);
        } else if (arg == "--debug_opengl") {
            Utils::setFlag(FLAG_DEBUG_OPENGL, true);
        } else if (arg == "--parent" || arg == "-p") {
            Utils::setFlag(FLAG_INSTALLATION_PARENT, true);
        } else if (arg == "--headless" || arg == "-hl") {
            Utils::setFlag(FLAG_HEADLESS, true);
        } else {
            std::cout << "Unknown arg " << arg << "\n";
        }
    }

    //Initialize log
    log_ptr log = Log::get(MAIN_LOG);

    //Initialize SDL2 and run if success
    std::string error;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0) {
        error = "SDL_Init failed\n" + Utils::checkSDLError();
    } else {
        engine->run();
        error = engine->getError();
        engine->close();
        engine.reset();
    }

    //Show any error
    if (!error.empty()) {
        Utils::showErrorDialog(error, log, false, true);
    }

    //Close SDL (doesn't matter if SDL was init successfully)
    SDL_Quit();
    Utils::checkSDLError(log); //Show any error but don't stop now

    //Close the logs
    Log::closeAll();

    //Restore original sighandler
    Utils::restoreSignalHandler();

    //Return code
    return error.empty() ? 0 : 1;
}

Engine::Engine() {
    log = Log::get("Engine");
}

Engine::~Engine() {
    close();
    log->debug("Destroyed");
}

void Engine::close() {
    log->debug("Closing");
    if (eventHandler) {
        eventHandler.reset();
    }
    if (guiRoot) {
        guiRoot.reset();
    }
    if (simulation) {
        simulation->close();
        simulation.reset();
    }
    if (renderer) {
        renderer.reset();
    }
    if (window) {
        window.reset();
    }
    if (entityManager) {
        entityManager.reset();
    }
    if (assetManager) {
        assetManager.reset();
    }
    if (updateTimer) {
        updateTimer.reset();
    }
    if (drawTimer) {
        drawTimer.reset();
    }
}

void Engine::run() {
    //Since log is created before setup we have to reset the default level
    Log::set_default_level(log);

    log->debug("Running");

    //Show some debug info
    log->debug("Install path: " + Utils::getInstallPath());
    log->debug("Data path: " + Utils::getDataPath());
    log->debug("User path: " + Utils::getUserPath());

    //Obtain own pointer
    std::shared_ptr<Engine> this_ptr = shared_from_this();

    //Initialize config
    loadConfig();
    if (hasError()) {
        return;
    }

    //Initialize locale
    setupLocale();
    if (hasError()) {
        return;
    }

    //Initialize timers
    updateTimer = std::make_unique<Timer>();
    drawTimer = std::make_unique<Timer>();

    //Initialize event handler
    eventHandler = std::make_unique<EventHandler>(this_ptr);
    setupEventHandler();
    if (hasError()) {
        return;
    }

    if (!Utils::isFlag(FLAG_HEADLESS)) {
        //Initialize window
        window = std::make_unique<Window>();
        error = window->getError();
        if (hasError()) {
            error = "Error initializing window\n" + error;
            return;
        }
        setupWindow();
        if (hasError()) {
            return;
        }

        //Initialize renderer
        renderer = std::make_unique<Renderer>();
        error = renderer->getError();
        if (hasError()) {
            error = "Error initializing renderer\n" + error;
            return;
        }
        setupRenderer();
        if (hasError()) {
            return;
        }
    }

    //Initialize asset manager
    assetManager = std::make_unique<AssetManager>(this_ptr);
    setupAssetManager();
    if (hasError()) {
        return;
    }

    //Initialize entity manager
    entityManager = std::make_unique<EntityManager>(this_ptr);
    setupEntityManager();

    //Setup GUI
    //TODO should show the main menu
    setupGUI();

    //Write config back
    saveConfig();
    if (hasError()) {
        return;
    }
}

void Engine::update() {
    //Check if we should skip update
    float updateTimerElapsed = updateTimer->elapsed();
    if (GAME_DELTA > updateTimerElapsed * 1000) {
        return;
    }
    updateElapsedAvg *= 0.8;
    updateElapsedAvg += (std::max(0.0001f, updateTimerElapsed) * 0.2f);
    updateTimer->update();

    //Update simulation
    if (simulation) {
        simulation->update();
    }

    //Update UI
    if (guiRoot) {
        guiRoot->update();
    }

    //Poll input
    eventHandler->poll();

    //Update event handlers
    eventHandler->eventUpdate();
}

void Engine::draw() {
    //Check if window and renderer is available
    if (!window || !renderer) {
        return;
    }

    //Clear
    window->clear();

    //Draw UI
    if (guiRoot) {
        renderer->changeCamera(0, 0);
        guiRoot->draw();
    }

    //Flush renderer
    renderer->flush();
    size_t flushes = renderer->flushes;
    renderer->flushes = 0;

    //Update timer and title
    drawElapsedAvg *= 0.8;
    drawElapsedAvg += (std::max(0.0001f, drawTimer->elapsed()) * 0.2f);
    int ups = static_cast<int>(std::round(1.0f / updateElapsedAvg));
    int fps = static_cast<int>(std::round(1.0f / drawElapsedAvg));
    window->setTitle(std::to_string(ups) + " UPS " + std::to_string(fps) + " FPS " + std::to_string(flushes) + " Flushes");
    drawTimer->update();

    //Update window content
    window->swap();
}

void Engine::setupEventHandler() {
}

void Engine::setupWindow() {
    //Setup sampling and vsync
    window->setSampling(getData<bool>("sampling", true));
    window->setVSync(getData<bool>("vsync", true));
}

void Engine::setupRenderer() {
}

void Engine::setupAssetManager() {
    //Load the assets
    assetManager->loadAssets();
    error = assetManager->getError();
    if (!error.empty()) return;
}

void Engine::setupEntityManager() {
    //Load the entity manager
    entityManager->load();
    error = entityManager->getError();
    if (!error.empty()) return;
}

void Engine::setupSimulation(std::unique_ptr<SimulationParameters> parameters) {
    //Create simulation instance
    simulation = std::make_unique<Simulation>(this_shared_ptr<Engine>(), std::move(parameters));
    error = simulation->getError();
    if (hasError()) return;

    //Load stuff before doing simulation load
    loadFactions();
}

void Engine::setupGUI() {
}

EventHandler* Engine::getEventHandler() {
    return eventHandler.get();
}

Window* Engine::getWindow() {
    return window.get();
}

Renderer* Engine::getRenderer() {
    return renderer.get();
}

AssetManager* Engine::getAssetManager() {
    return assetManager.get();
}

EntityManager* Engine::getEntityManager() {
    return entityManager.get();
}

Simulation* Engine::getSimulation() {
    return simulation.get();
}

input_key_code_t Engine::getKeyBind(const std::string& name) {
    //TODO there should be a configurable keybinds and falling back to default if not set

    return EventHandler::getCodeFromName(name);
}

void Engine::loadConfig() {
    //Load any existing config
    Config config(Utils::getUserPath() + "config.json");
    config.read();
    error = config.getError();
    if (hasError()) {
        //Failed to read, use clear state
        log->warn("Couldn't read config: {0}", getError());
        config.clear();
    }
    loadData(config.data);
}

void Engine::saveConfig() {
    //Skip if is not dirty
    if (!unsetDataDirty()) {
        return;
    }
    log->debug("Saving dirty config");

    //Save config
    Config config(Utils::getUserPath() + "config.json");
    config.data.update(configData);
    config.write();
    error = config.getError();
}

void Engine::setupLocale() {
    //Load declared locales
    Config config(Utils::getDataPath() + "locales.json");
    config.read();
    error = config.getError();
    if (hasError()) {
        return;
    }
    config_data_t data = config.data;
    if (!data.is_object()) {
        error = "Locales config root is not object";
        return;
    }

    //Load available locales
    for (config_data_t::iterator entry = data.begin(); entry != data.end(); ++entry) {
        if (entry.key() != "default") {
            locales[entry.key()] = entry.value().get<std::string>();
        }
    }

    //Load the current locale from config
    std::string code = getData<const std::string>("locale", "");

    //Check if locale is valid and fallback to default locale
    bool persistent = false;
    if (code.empty() || locales.find(code) == locales.end()) {
        if (!code.empty()) {
            log->warn("Unknown locale: {0}", code);
        }
        code = data.value("default", "");
        persistent = true;
    }

    //Set it
    setLocale(code, persistent);
}

void Engine::setLocale(const std::string& code, bool persistent) {
    if (code.empty() || locales.find(code) == locales.end()) {
        error = "Locale not available: " + code;
        return;
    }

    //Load the config as current
    std::string name = locales[code];
    locale = std::make_unique<Locale>(code, name, Utils::getDataPath() + "locales" + DIR_SEP + code + ".json");
    log->debug("Current locale: {0}", locale->toString());

    //Write down
    if (persistent) {
        setData("locale", locale->code);
    }
}

void Engine::loadFactions() {
    //Load factions
    Config config(Utils::getDataPath() + "factions.json");
    config.read();
    error = config.getError();
    if (hasError()) {
        return;
    }

    //Load each faction config
    for (nlohmann::json& data : config.data) {
        entity_type_id_t id = data.value("id", 0);
        std::unique_ptr<Faction> faction = std::make_unique<Faction>(id);
        faction->code = data.value("code", "");
        faction->name = getText("faction."+faction->code);
        faction->loadData(data["data"]);
        simulation->addFaction(std::move(faction));
    }
}

const std::string& Engine::getText(const std::string& key) {
    if (locale) {
        const std::string& text = locale->getText(key);
        if (!text.empty()) {
            return text;
        }
    }
    return key;
}

void Engine::setGUI(std::shared_ptr<GUIRoot> root) {
    if (guiRoot) {
        eventHandler->removeEventListener(guiRoot);
        guiRoot->rootActive(std::shared_ptr<Engine>());
        guiRoot.reset();
    }
    if (root) {
        guiRoot = std::move(root);
        guiRoot->rootActive(this_shared_ptr<Engine>());
        eventHandler->addEventListener(guiRoot);
    }
}
