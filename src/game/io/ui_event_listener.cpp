//
// Created by Ion Agorria on 28/04/19
//

#include "game/core/game.h"
#include "ui_event_listener.h"

UIEventListener::UIEventListener(std::shared_ptr<Game> game): game(game) {
}

UIEventListener::~UIEventListener() {
    if (game) {
        game.reset();
    }
}

bool UIEventListener::mouseMove(Window* window, int x, int y) {
    return false;
}

bool UIEventListener::keyChange(Window* window, int code, bool press) {
    if (!press) {
    }
    return false;
}
