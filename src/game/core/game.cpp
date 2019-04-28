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
#include "game/assets/asset_processor_mix.h"
#include "game/io/ui_event_listener.h"
#include "game.h"

//TODO remove
std::unique_ptr<Palette> extra;
std::shared_ptr<Image> image;
int v = 90;
double t;

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
    assetManager->addAssetProcessor(std::make_unique<AssetProcessorMIX>());
    assetManager->addAssetProcessor(std::make_unique<AssetProcessorDatPal>());

    //Call setup
    Engine::setupAssetManager();
}

void Game::run() {
    Engine::run();

    //Show main window
    window->show();

    //TODO remove
    extra = std::make_unique<Palette>(10, true);
    test(0);

    //Main loop
    log->debug("Starting loop");
    while (!eventHandler->isClosing()) {
        loop();
    }
}

void Game::loop() {
    //Clear
    window->clear();

    //Poll input
    eventHandler->poll();

    //Update simulation

    //Draw the simulation

    //Draw/update UI
    renderer->changeCamera(0, 0);
    //TODO remove this
    t += 0.05;
    if (image) {
        ColorRGBA color = {(byte) (std::round(std::sin(t) * 0x7f) + 0x7f), 0,
                           (byte) (std::round(std::cos(t) * 0x7f) + 0x7f), 128};
        for (unsigned int i = 0; i < extra->length(); ++i) {
            extra->setColor(i, color);
        }
        extra->updateTexture();
        for (float j = 0; j <= 39; j += 1) {
            for (float i = 0; i <= 28; i += 1) {
                renderer->draw(j * 10, i * 10, 50, 50, std::sin(t / 2) * 1.3, *image, extra.get());
            }
        }
    }
    renderer->flush();

    //Update window
    window->swap();
}

void Game::test(int i) {
    if (0 < i) {
        v++;
    } else if (i < 0) {
        v--;
    }
    std::string path = v < 0 ? "MIX/SPRT0/"+std::to_string(std::abs(v) - 1) : "MIX/SPRU0/"+std::to_string(v);
    log->debug(path);
    image = assetManager->getImage(path);
    if (image) log->debug("Current: {0} {1}", v, image->toString());
}