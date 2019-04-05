//
// Created by Ion Agorria on 20/05/18
//
#include "eventhandler.h"
#include "core/game.h"

EventHandler::EventHandler(std::shared_ptr<Game> game): game(game) {
    log = Log::get("EventHandler");
}

EventHandler::~EventHandler() {
    log->debug("Closing");
    if (game) {
        game.reset();
    }
}

void EventHandler::windowResize(int width, int height) {
    log->debug("Window size {0}x{1}", width, height);
}

void EventHandler::mouseClick(int x, int y, int button, bool press) {
    log->debug("Mouse button: {0} at {1}x{2} {3}", button, x, y, press ? "press" : "release");
}

void EventHandler::mouseMove(int x, int y) {
    //log->debug("Mouse motion: {0}x{1}", x, y);
}

void EventHandler::keyChange(int code, std::string name, bool press) {
    log->debug("Key change: {0} '{1}' {2}", code, name, press ? "press" : "release");
    if (press) return;
    if (name == "Left") {
        game->test(-1);
    } else if (name == "Right") {
        game->test(1);
    }
}
