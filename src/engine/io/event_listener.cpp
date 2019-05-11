//
// Created by Ion Agorria on 28/04/19
//

#include "graphics/window.h"
#include "event_listener.h"

bool IEventListener::windowChanged(Window* window) {
    return false;
}

bool IEventListener::mouseClick(Window* window, int x, int y, int button, bool press) {
    return false;
}

bool IEventListener::mouseWheel(Window* window, int x, int y) {
    return false;
}

bool IEventListener::mouseMove(Window* window, int x, int y) {
    return false;
}

bool IEventListener::keyChange(Window* window, int code, bool press) {
    return false;
}
