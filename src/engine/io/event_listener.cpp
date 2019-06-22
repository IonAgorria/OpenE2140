//
// Created by Ion Agorria on 28/04/19
//

#include "engine/graphics/window.h"
#include "event_listener.h"

bool IEventListener::update() {
    return false;
}

bool IEventListener::eventWindowChanged(Window* window) {
    return false;
}

bool IEventListener::eventWindowFocus(Window* window, bool state) {
    return false;
}

bool IEventListener::eventMouseClick(Window* window, int x, int y, int button, bool press) {
    return false;
}

bool IEventListener::eventMouseWheel(Window* window, int x, int y) {
    return false;
}

bool IEventListener::eventMouseMove(Window* window, int x, int y) {
    return false;
}

bool IEventListener::eventKeyChange(Window* window, input_key_t& key) {
    return false;
}
