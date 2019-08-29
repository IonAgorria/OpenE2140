//
// Created by Ion Agorria on 28/04/19
//
#include "engine/core/utils.h"
#include "engine/graphics/window.h"
#include "event_dispatcher.h"

void EventDispatcher::addEventListener(std::shared_ptr<IEventListener> listener) {
    listeners.push_back(std::move(listener));
}

void EventDispatcher::removeEventListener(std::shared_ptr<IEventListener> listener) {
    Utils::eraseElementFromVector(listeners, std::move(listener));
}

bool EventDispatcher::eventUpdate() {
    bool handled = false;
    for (auto& listener : listeners) {
        handled = listener->eventUpdate();
        if (handled) {
            break;
        }
    }
    return handled;
}

bool EventDispatcher::eventWindowChanged(Window* window) {
    bool handled = false;
    for (auto& listener : listeners) {
        handled = listener->eventWindowChanged(window);
        if (handled) {
            break;
        }
    }
    return handled;
}

bool EventDispatcher::eventWindowFocus(Window* window, bool state) {
    bool handled = false;
    for (auto& listener : listeners) {
        handled = listener->eventWindowFocus(window, state);
        if (handled) {
            break;
        }
    }
    return handled;
}

bool EventDispatcher::eventMouseClick(Window* window, int x, int y, int button, bool press) {
    bool handled = false;
    for (auto& listener : listeners) {
        handled = listener->eventMouseClick(window, x, y, button, press);
        if (handled) {
            break;
        }
    }
    return handled;
}

bool EventDispatcher::eventMouseWheel(Window* window, int x, int y) {
    bool handled = false;
    for (auto& listener : listeners) {
        handled = listener->eventMouseWheel(window, x, y);
        if (handled) {
            break;
        }
    }
    return handled;
}

bool EventDispatcher::eventMouseMove(Window* window, int x, int y) {
    bool handled = false;
    for (auto& listener : listeners) {
        handled = listener->eventMouseMove(window, x, y);
        if (handled) {
            break;
        }
    }
    return handled;
}

bool EventDispatcher::eventKeyChange(Window* window, input_key_t& key) {
    bool handled = false;
    for (auto& listener : listeners) {
        handled = listener->eventKeyChange(window, key);
        if (handled) {
            break;
        }
    }
    return handled;
}