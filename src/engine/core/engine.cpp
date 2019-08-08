//
// Created by Ion Agorria on 1/11/18
//
#include <SDL.h>
#include "engine/core/common.h"
#include "engine/assets/asset_manager.h"
#include "engine/graphics/palette.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/window.h"
#include "engine/gui/guimenu.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/faction.h"
#include "engine/simulation/world/world.h"
#include "engine/simulation/entities/entity_manager.h"
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
        if (arg.compare("-debug") == 0 || arg.compare("-d") == 0) {
            Utils::setFlag(FLAG_DEBUG, true);
        } else if (arg.compare("-parent") == 0) {
            Utils::setFlag(FLAG_INSTALLATION_PARENT, true);
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
    if (menu) {
        menu.reset();
    }
    if (simulation) {
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
    if (timer) {
        timer.reset();
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

    //Initialize timer
    timer = std::make_unique<Timer>();

    //Initialize event handler
    eventHandler = std::make_unique<EventHandler>(this_ptr);
    setupEventHandler();
    if (hasError()) {
        return;
    }

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

    //Initialize asset manager
    assetManager = std::make_unique<AssetManager>(this_ptr);
    setupAssetManager();
    if (hasError()) {
        return;
    }

    //Initialize entity manager
    entityManager = std::make_unique<EntityManager>(this_ptr);
    setupEntityManager();

    //Write config back
    saveConfig();
    if (hasError()) {
        return;
    }
}

void Engine::update() {
    //Update timer
    timer->update();

    //Poll input
    eventHandler->poll();

    //Update event handlers
    eventHandler->update();

    //Update simulation
    if (simulation) {
        simulation->update();
    }
}

void Engine::draw() {
    //Clear
    window->clear();

    //Draw the simulation if any
    if (simulation) {
        Rectangle viewport = renderer->getViewport();
        viewport.setPosition(camera);
        renderer->changeCamera(camera.x, camera.y);
        simulation->draw(viewport);
    }

    //Draw/update UI
    if (menu) {
        renderer->changeCamera(0, 0);
        menu->draw();
    }

    //Flush renderer
    renderer->flush();

    //Update window
    float elapsed = std::max(0.001f, timer->elapsed());
    window->setTitle(std::to_string(1.0f / elapsed).substr(0, 4) + " FPS");
    window->swap();
}

void Engine::setupEventHandler() {
}

void Engine::setupWindow() {
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

void Engine::setupSimulation(std::unique_ptr<SimulationParameters>& parameters) {
    simulation = std::make_unique<Simulation>(this_shared_ptr<Engine>(), parameters);
    error = simulation->getError();

    //Load stuff before doing simulation load
    loadFactions();

    //Load the simulation
    simulation->load();
    error = simulation->getError();
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

Vector2& Engine::getCamera() {
    return camera;
}

void Engine::updateCamera(const Vector2& newCamera) {
    this->camera.set(newCamera);

    //Limit camera movement by world bounds
    if (simulation && renderer) {
        Rectangle viewport = renderer->getViewport();
        Rectangle worldRectangle = simulation->getWorld()->getWorldRectangle();
        Vector2 max(
            worldRectangle.w - viewport.w,
            worldRectangle.h - viewport.h
        );
        if (camera.x < worldRectangle.x) camera.x = worldRectangle.x;
        if (camera.y < worldRectangle.y) camera.y = worldRectangle.y;
        if (max.x < camera.x) camera.x = max.x;
        if (max.y < camera.y) camera.y = max.y;
    }

    //Never let to be less than 0
    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
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
    std::string code = getData<const std::string>("locale", ""); //TODO load this from config

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
        std::unique_ptr<Faction> faction = std::make_unique<Faction>();
        faction->id = id;
        faction->code = data.value("code", "");
        faction->name = getText("faction."+faction->code);
        faction->loadData(data["data"]);
        simulation->addFaction(std::move(faction));
    }
}

const std::string Engine::getText(const std::string& key) {
    if (locale) {
        const std::string& text = locale->getText(key);
        if (!text.empty()) {
            return text;
        }
    }
    return "?_" + key + "_?";
}
