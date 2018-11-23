//
// Created by Ion Agorria on 1/11/18
//
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
    if (window) {
        window.reset();
    }
    if (luaVM) {
        luaVM.reset();
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
    window = std::make_unique<Window>();
    if (!window->init(DEFAULT_RESOLUTION_WIDTH, DEFAULT_RESOLUTION_HEIGHT, GAME_TITLE, *eventHandler)) {
        return false;
    }

    //Initialize asset manager
    std::unique_ptr<Manager> manager = std::make_unique<Manager>();
    if (!manager->init()) {
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
    //Poll input
    window->poll(*eventHandler);

    //Update simulation
    //simulation

    //Draw the simulation
    //renderer->()

    //Draw/update UI


    //Update window
    window->swap();
}
