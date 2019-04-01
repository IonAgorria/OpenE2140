//
// Created by Ion Agorria on 1/11/18
//
#include "core/common.h"
#include "assets/assetmanager.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "gui/eventhandler.h"
#include "simulation/simulation.h"
#include "core/luavm.h"
#include "game.h"

Game::Game() {
    log = Log::get("Game");
}

Game::~Game() {
    close();
    log->debug("Destroyed");
}

void Game::close() {
    log->debug("Closing");
    if (assetManager) {
        assetManager.reset();
    }
    if (renderer) {
        renderer.reset();
    }
    if (window) {
        window.reset();
    }
    if (luaVM) {
        luaVM.reset();
    }
    if (eventHandler) {
        eventHandler.reset();
    }
}

std::shared_ptr<Image> image;
bool Game::run() {
    log->debug("Running");

    std::shared_ptr<Game> this_ptr = shared_from_this();

    // Initialize Lua
    luaVM = LuaVM::create();

    //Initialize event handler
    eventHandler = std::make_unique<EventHandler>(this_ptr);

    //Initialize window
    window = std::make_unique<Window>(DEFAULT_RESOLUTION_WIDTH, DEFAULT_RESOLUTION_HEIGHT, GAME_TITLE, *eventHandler);
    std::string error = window->getError();
    if (!error.empty()) {
        log->error("Error initializing window\n{0}", error);
        return false;
    }

    // Initialize renderer
    renderer = std::make_unique<Renderer>();
    error = renderer->getError();
    if (!error.empty()) {
        log->error("Error initializing renderer\n{0}", error);
        return false;
    }

    //Initialize asset manager
    assetManager = std::make_unique<AssetManager>(this_ptr);
    assetManager->loadAssets();
    error = assetManager->getError();
    if (!error.empty()) {
        log->error("Error initializing asset manager\n{0}", error);
        return false;
    }

    image = assetManager->getImage("MIX/SPRT2/5");

    //Main loop
    log->debug("Starting loop");
    while (!window->isClosing()) {
        loop();
    }

    //Close myself
    close();

    return true;
}

void Game::loop() {
    //Clear
    window->clear();

    //Poll input
    window->poll(*eventHandler);

    //Update simulation

    //Draw the simulation

    //Draw/update UI

    //TODO remove this
    renderer->draw(0, 0, 1, 1, 1.2, *image, nullptr);
    renderer->flush();

    //Update window
    window->swap();
}

void loop();

/** @return Window */
Window* Game::getWindow() {
    return window.get();
}

/** @return Simulation */
Simulation* Game::getSimulation() {
    return simulation.get();
}

/** @return AssetManager */
AssetManager* Game::getAssetManager() {
    return assetManager.get();
}