//
// Created by Ion Agorria on 27/04/19
//
#include "engine/core/common.h"
#include "engine/assets/asset_manager.h"
#include "engine/graphics/palette.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/window.h"
#include "src/engine/io/event_handler.h"
#include "engine/simulation/simulation.h"
#include "engine/core/utils.h"
#include "game/assets/asset_processor_wd.h"
#include "game/assets/asset_processor_datpal.h"
#include "game/assets/asset_processor_level.h"
#include "game/assets/asset_processor_mix.h"
#include "game/io/ui_event_listener.h"
#include "game.h"

void Game::setupEventHandler() {
    //Register event listeners
    std::shared_ptr<Game> this_ptr = this_shared_ptr<Game>();
    eventHandler->addEventListener(std::make_unique<UIEventListener>(this_ptr));

    //Call setup
    Engine::setupEventHandler();
}

void Game::setupAssetManager() {
    //Register processors
    assetManager->addAssetProcessor(std::make_unique<AssetProcessorWD>());
    assetManager->addAssetProcessor(std::make_unique<AssetProcessorDatPal>());
    assetManager->addAssetProcessor(std::make_unique<AssetProcessorLevel>());
    assetManager->addAssetProcessor(std::make_unique<AssetProcessorMIX>());

    //Call setup
    Engine::setupAssetManager();
}

void Game::run() {
    Engine::run();
    if (hasError()) {
        return;
    }

    //TODO
    std::unique_ptr<SimulationParameters> parameters = std::make_unique<SimulationParameters>();
    parameters->seed = 1;
    parameters->world = "LEVEL/DATA/LEVEL01.DAT";
    setupSimulation(parameters);
    if (hasError()) {
        return;
    }

    //Show main window
    window->show();

    //Main loop
    log->debug("Starting loop");
    while (!eventHandler->isClosing()) {
        update();
        draw();
    }
}