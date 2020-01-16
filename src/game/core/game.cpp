//
// Created by Ion Agorria on 27/04/19
//
#include "engine/entities/entity_manager.h"
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
#include "game/assets/asset_processor_fixes.h"
#include "game/entities/factories.h"
#include "game/io/event_listener_debug.h"
#include "game/gui/game_layout.h"
#include "game.h"

number_t Game::SpriteRotationCorrection = 0;

void Game::setupEventHandler() {
    //Register event listeners
    std::shared_ptr<Game> this_ptr = this_shared_ptr<Game>();
    if (Utils::isFlag(FLAG_DEBUG)) {
        eventHandler->addEventListener(std::make_shared<EventListenerDebug>(this_ptr));
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
    assetManager->addAssetProcessor(std::make_unique<AssetProcessorFixes>());

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
    entityManager->addEntityFactory(std::make_unique<UnitFactory>());
    entityManager->addEntityFactory(std::make_unique<BuildingFactory>());
    entityManager->addEntityFactory(std::make_unique<AttachmentFactory>());

    //Call setup
    Engine::setupEntityManager();
}

void Game::setupSimulation(std::unique_ptr<SimulationParameters> parameters) {
    //Call setup
    Engine::setupSimulation(std::move(parameters));
    if (hasError()) return;

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

void Game::setupGUI() {
    //Call setup
    Engine::setupGUI();
}

void Game::run() {
    Engine::run();
    if (hasError()) {
        return;
    }

    //Setup static stuff
    SpriteRotationCorrection = number_div(NUMBER_PI, int_to_number(2));

    //Prepare simulation
    //TODO this is only for testings
    std::unique_ptr<SimulationParameters> parameters = std::make_unique<SimulationParameters>();
    parameters->seed = 1;
    parameters->loadLevelContent = true;
    parameters->world = "LEVEL/DATA/LEVEL01";
    parameters->world = "LEVEL/DATA/LEVEL06";
    //parameters->world = "LEVEL/DATA/LEVEL351";
    //parameters->world = "LEVEL/DATA/LEVEL334";
    //parameters->world = "LEVEL2/DATA/LEVEL511";
    std::unique_ptr<Player> player = std::make_unique<Player>(1);
    player->color = {{0x60, 0xA0, 0x20, 0xFF}};
    parameters->players.emplace_back(std::move(player));
    player = std::make_unique<Player>(2);
    player->color = {{0xFF, 0x40, 0x40, 0xFF}};
    parameters->players.emplace_back(std::move(player));

    setupSimulation(std::move(parameters));
    if (hasError()) {
        return;
    }

    //Create some entities
    Player* playerPtr = simulation->getPlayer(1);
    std::shared_ptr<Entity> entityPtr = entityManager->makeEntity({ENTITY_KIND_BUILDING, 19});
    entityPtr->setPosition({64 * 8 + 32, 64 * 8 + 32});
    PlayerComponent* component = GET_COMPONENT_DYNAMIC(entityPtr.get(), PlayerComponent);
    component->setPlayer(playerPtr);
    simulation->addEntity(entityPtr);
    auto tile = simulation->getWorld()->getTile(10, 2);
    unsigned int y = 0;
    for (unsigned int i = 41; i <= 85; ++i) {
        entityPtr = entityManager->makeEntity({ENTITY_KIND_UNIT, i});
        if (!entityPtr) continue;
        entityPtr->setPosition({
            static_cast<signed>(64 * ((y % 8) + 3) + 32),
            static_cast<signed>(64 * ((y / 8) + 3) + 32)
        });
        if (i == 41) {
            entityPtr->setPosition({
               static_cast<signed>(32 + 64 * 2),
               static_cast<signed>(32 + 64 * 2)
            });
        }
        component = GET_COMPONENT_DYNAMIC(entityPtr.get(), PlayerComponent);
        component->setPlayer(playerPtr);
        simulation->addEntity(entityPtr);
        y++;

        //Test pathfinder
        if (i == 41) {
            MovementComponent* movement = GET_COMPONENT_DYNAMIC(entityPtr.get(), MovementComponent);
            movement->move(tile);
        }
    }

    //Do like we are launching the game GUI
    auto gameLayout = std::make_shared<GameLayout>();
    gameLayout->setUserPlayer(playerPtr);
    setGUI(gameLayout);

    //Show main window
    if (window) {
        window->show();
    }

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

bool Game::angleToSpriteIndex(number_t angle, number_t halfSide, uint16_t& index) {
    //Apply correction
    angle = number_wrap_angle(number_add(angle, SpriteRotationCorrection));
    //Set ccw flag, negative north and south sprites shouldn't have ccw set
    bool ccw = angle < NUMBER_ZERO;
    //Convert angle to index
    angle = number_div(number_abs(angle), halfSide);
    index = number_to_int(number_floor(angle));
    //Convert half slices to proper one
    if (0 < index) {
        index = (index + 1) / 2;
    }
    return ccw;
}

number_t Game::angleToSpriteAngle(number_t angle, number_t side) {
    //Do an approximate rounding
    int16_t round = number_to_int(number_div(angle, side));
    angle = number_mul(int_to_number(round), side);
    return angle;
}