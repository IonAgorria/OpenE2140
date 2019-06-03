//
// Created by Ion Agorria on 28/04/19
//

#include "game/core/constants.h"
#include "game/core/game.h"
#include "engine/assets/asset_manager.h"
#include "event_listener_debug.h"

EventListenerDebug::EventListenerDebug(std::shared_ptr<Game> game): game(game) {
    keyAssetsReload = game->getKeyBind("F12");
}

EventListenerDebug::~EventListenerDebug() {
    if (game) {
        game.reset();
    }
}

bool EventListenerDebug::keyChange(Window* window, input_key_t& key, bool press) {
    if (press) {
        if (key.code == keyAssetsReload) {
            //game->getAssetManager()->loadAssets();
            game->getAssetManager()->refreshAssets();
        }
    }
    return false;
}