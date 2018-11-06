//
// Created by Ion Agorria on 1/11/18
//
#include "game.h"

Game::Game() {
    log = Log::get("Game");
}

Game::~Game() {
    log->debug("Closing");
    assetManager.reset();
    window.reset();
    luaVM.reset();
}

bool Game::run() {
    log->debug("Creating");

    // Initialize Lua
    luaVM = LuaVM::create();

    //Initialize event handler
    EventHandler eventHandler(*this);

    //Initialize window
    window = std::make_unique<Window>(eventHandler);
    if (!window->init(DEFAULT_RESOLUTION_WIDTH, DEFAULT_RESOLUTION_HEIGHT, GAME_TITLE)) {
        return false;
    }

    //Initialize asset manager
    std::unique_ptr<Manager> manager = std::make_unique<Manager>();
    if (!manager->init()) {
        return false;
    }

    //Main loop
    log->debug("Starting loop");
    bool error = false;
    while (!window->isClosing()) {
        //

        //Update window
        if (!window->update()) {
            error = true;
            break;
        }
    }

    return !error;
}
