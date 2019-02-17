//
// Created by Ion Agorria on 1/11/18
//
#include "game.h"
#include "core/common.h"

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

bool Game::run() {
    log->debug("Running");

    std::shared_ptr<Game> this_ptr = shared_from_this();//this_shared_ptr();

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
    assetManager = std::make_unique<AssetManager>();
    error = assetManager->getError();
    if (!error.empty()) {
        log->error("Error initializing asset manager\n{0}", error);
        return false;
    }

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
    //renderer->draw(0, 0, 0.5, 0.8, 1.2, );
    renderer->flush();

    //Update window
    window->swap();
}
