//
// Created by Ion Agorria on 27/04/19
//
#include "engine/simulation/entities/entity_manager.h"
#include "engine/graphics/window.h"
#include "engine/simulation/player.h"
#include "engine/simulation/faction.h"
#include "engine/simulation/world/world.h"
#include "engine/simulation/world/tile.h"
#include "engine/core/utils.h"
#include "game/assets/asset_processor_wd.h"
#include "game/assets/asset_processor_datpal.h"
#include "game/assets/asset_processor_level.h"
#include "game/assets/asset_processor_mix.h"
#include "game/entities/factories.h"
#include "game/io/event_listener_camera.h"
#include "game/io/event_listener_debug.h"
#include "game.h"

void Game::setupEventHandler() {
    //Register event listeners
    std::shared_ptr<Game> this_ptr = this_shared_ptr<Game>();
    eventHandler->addEventListener(std::make_unique<EventListenerCamera>(this_ptr));
    if (Utils::isFlag(FLAG_DEBUG)) {
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

void Game::setupEntityManager() {
    //Register factories
    entityManager->addEntityFactory(std::make_unique<ObjectFactory>());
    entityManager->addEntityFactory(std::make_unique<BuildingFactory>());

    //Call setup
    Engine::setupEntityManager();
}

void Game::setupSimulation(std::unique_ptr<SimulationParameters> parameters) {
    //Call setup
    Engine::setupSimulation(std::move(parameters));

    //Load the simulation
    simulation->loadWorld();
    error = simulation->getError();
    if (hasError()) return;

    simulation->loadPlayers();
    error = simulation->getError();
    if (hasError()) return;
    setupPlayerColors();

    simulation->loadEntities();
    error = simulation->getError();
    if (hasError()) return;
}

void Game::run() {
    Engine::run();
    if (hasError()) {
        return;
    }

    //Prepare simulation
    //TODO
    std::unique_ptr<SimulationParameters> parameters = std::make_unique<SimulationParameters>();
    parameters->seed = 1;
    parameters->loadLevelContent = true;
    parameters->world = "LEVEL/DATA/LEVEL02";
    parameters->world = "LEVEL/DATA/LEVEL06";
    //parameters->world = "LEVEL/DATA/LEVEL351";
    //parameters->world = "LEVEL/DATA/LEVEL334";
    std::unique_ptr<Player> player = std::make_unique<Player>(1);
    player->color = {0x60, 0xA0, 0x20, 0xFF};
    parameters->players.emplace_back(std::move(player));
    player = std::make_unique<Player>(2);
    player->color = {0xFF, 0x40, 0x40, 0xFF};
    parameters->players.emplace_back(std::move(player));

    setupSimulation(std::move(parameters));
    if (hasError()) {
        return;
    }

    //Create some entities
    std::shared_ptr<Entity> entityPtr = entityManager->makeEntity({2, 19});
    entityPtr->setPosition({32, 64 * 7 + 32});
    PlayerComponent* component = GET_COMPONENT(entityPtr.get(), PlayerComponent);
    component->setPlayer(simulation->getPlayer(1));
    simulation->addEntity(entityPtr);
    entityPtr = entityManager->makeEntity({0, 207});
    entityPtr->setPosition({32, 32});
    simulation->addEntity(entityPtr);
    entityPtr = entityManager->makeEntity({0, 207});
    entityPtr->setPosition({32, 64 + 32});
    dynamic_cast<Spinner*>(entityPtr.get())->clockwise = false;
    simulation->addEntity(entityPtr);
    entityPtr = entityManager->makeEntity({0, 206});
    entityPtr->setPosition({64 + 32, 64 + 32});
    simulation->addEntity(entityPtr);

    //Show main window
    window->show();

    //Main loop
    log->debug("Starting loop");
    while (!eventHandler->isClosing()) {
        update();
        draw();
    }
}

void Game::setupPlayerColors() {
    //Generate player palette colors using base color
    for (std::unique_ptr<Player>& player : simulation->getPlayers()) {
        if (!player) continue;
        ColorHSV base;
        base.fromRGB(player->color);
        player->extraColors.clear();
        for (ColorHSV hsv : Color::PLAYER) {
            hsv.h = hsv.h + base.h;
            hsv.s = hsv.s * base.s;
            hsv.v = hsv.v * base.v;
            hsv.check();
            ColorRGBA paletteColor;
            hsv.toRGB(paletteColor);
            paletteColor.a = player->color.a;
            player->extraColors.emplace_back(paletteColor);
        }
    }
}

void Game::setReactorCrate(Tile& tile) {
    BIT_OFF(tile.tileFlags, TILE_FLAG_PASSABLE);
    BIT_ON(tile.tileFlags, TILE_FLAG_IMMUTABLE);
    tile.isImageDirty = true;
    //TODO set damage type and destroy any entity inside
    //TODO mark the surrounding tiles a radiactive
}