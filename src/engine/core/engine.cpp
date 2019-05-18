//
// Created by Ion Agorria on 1/11/18
//
#include <SDL.h>
#include "core/common.h"
#include "assets/asset_manager.h"
#include "graphics/palette.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "gui/guimenu.h"
#include "simulation/simulation.h"
#include "engine/io/event_handler.h"
#include "engine/core/utils.h"
#include "engine/io/timer.h"
#include "engine.h"

int Engine::main(int argc, char** argv, std::shared_ptr<Engine> engine) {
    //Register signal and terminate handler
    Utils::setSignalHandler(Utils::handleHaltAndCatchFire, Utils::handleTerminate);

    //Cache the paths
    Utils::getInstallPath();
    Utils::getUserPath();

    //Parse args
    for(int i=1; i < argc; i++) {
        std::string arg = argv[i];
        std::transform(BEGIN_END(arg), arg.begin(), ::tolower);
        if (arg.compare("-debug") == 0 || arg.compare("-d") == 0) {
            Utils::setDebug(true);
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

    std::shared_ptr<Engine> this_ptr = shared_from_this();

    //Initialize timer
    timer = std::make_unique<Timer>();

    //Initialize event handler
    eventHandler = std::make_unique<EventHandler>(this_ptr);

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

    // Initialize renderer
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
}

void Engine::update() {
    //Update timer
    timer->update();

    //Poll input
    eventHandler->poll();

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
        simulation->draw(renderer->getViewport());
    }

    //Draw/update UI
    if (menu) {
        menu->draw();
    }

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
    //Load assets
    assetManager->loadAssets(std::string(GAME_ASSETS_DIR) + DIR_SEP, GAME_ASSET_CONTAINER_NAMES);
    error = assetManager->getError();
    if (hasError()) {
        error = "Error initializing asset manager\n" + error;
        return;
    }
}

void Engine::setupSimulation(std::unique_ptr<SimulationParameters> parameters) {
    simulation = std::make_unique<Simulation>(this_shared_ptr<Engine>(), parameters);
    error = simulation->getError();
    if (hasError()) {
        error = "Error initializing simulation\n" + error;
    }
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

Simulation* Engine::getSimulation() {
    return simulation.get();
}

AssetManager* Engine::getAssetManager() {
    return assetManager.get();
}