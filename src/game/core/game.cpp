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
#include "game/io/event_listener_camera.h"
#include "game/io/event_listener_debug.h"
#include "game.h"

void Game::setupEventHandler() {
    //Register event listeners
    std::shared_ptr<Game> this_ptr = this_shared_ptr<Game>();
    eventHandler->addEventListener(std::make_unique<EventListenerCamera>(this_ptr));
    if (Utils::isDebug()) {
        eventHandler->addEventListener(std::make_unique<EventListenerDebug>(this_ptr));
    }

    //Call setup
    Engine::setupEventHandler();
}

void Game::setupAssetManager() {
    //Register processors
    assetManager->addAssetProcessor(std::make_unique<AssetProcessorWD>());
    assetManager->addAssetProcessor(std::make_unique<AssetProcessorDatPal>());
    assetManager->addAssetProcessor(std::make_unique<AssetProcessorLevel>());
    assetManager->addAssetProcessor(std::make_unique<AssetProcessorMIX>());

    //Load assets
    assetManager->registerAssetContainer("PIRO", true);
    assetManager->registerAssetContainer("MIX", true);
    assetManager->registerAssetContainer("LEVEL", true);
    assetManager->registerAssetContainer("LEVEL2", false);
    error = assetManager->getError();
    if (hasError()) {
        return;
    }

    //Call setup
    Engine::setupAssetManager();
}

void Game::run() {
    Engine::run();
    if (hasError()) {
        return;
    }

    //TODO
    //camera.set(100, 100);
    std::unique_ptr<SimulationParameters> parameters = std::make_unique<SimulationParameters>();
    parameters->seed = 1;
    parameters->world = "LEVEL/DATA/LEVEL02";
    parameters->world = "LEVEL/DATA/LEVEL06";
    parameters->world = "LEVEL/DATA/LEVEL351";
    //parameters->world = "LEVEL/DATA/LEVEL334";
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