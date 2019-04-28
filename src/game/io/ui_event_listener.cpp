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

bool UIEventListener::keyChange(Window* window, int code, const std::string& name, bool press) {
    if (!press) {
        if (name == "Left") {
            game->test(-1);
        } else if (name == "Right") {
            game->test(1);
        }
    }
}
