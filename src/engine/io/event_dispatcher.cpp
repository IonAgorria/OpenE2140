//
// Created by Ion Agorria on 28/04/19
//
#include "graphics/window.h"
#include "event_dispatcher.h"

bool EventDispatcher::windowChanged(Window* window) {
    bool handled = false;
    for (std::unique_ptr<IEventListener>& listener : listeners) {
        handled = listener->windowChanged(window);
        if (handled) {
            break;
        }
    }
    return handled;
}

bool EventDispatcher::windowFocus(Window* window, bool state) {
    bool handled = false;
    for (std::unique_ptr<IEventListener>& listener : listeners) {
        handled = listener->windowFocus(window, state);
        if (handled) {
            break;
        }
    }
    return handled;
}

bool EventDispatcher::mouseClick(Window* window, int x, int y, int button, bool press) {
    bool handled = false;
    for (std::unique_ptr<IEventListener>& listener : listeners) {
        handled = listener->mouseClick(window, x, y, button, press);
        if (handled) {
            break;
        }
    }
    return handled;
}

bool EventDispatcher::mouseWheel(Window* window, int x, int y) {
    bool handled = false;
    for (std::unique_ptr<IEventListener>& listener : listeners) {
        handled = listener->mouseWheel(window, x, y);
        if (handled) {
            break;
        }
    }
    return handled;
}

bool EventDispatcher::mouseMove(Window* window, int x, int y) {
    bool handled = false;
    for (std::unique_ptr<IEventListener>& listener : listeners) {
        handled = listener->mouseMove(window, x, y);
        if (handled) {
            break;
        }
    }
    return handled;
}

bool EventDispatcher::keyChange(Window* window, int code, bool press) {
    bool handled = false;
    for (std::unique_ptr<IEventListener>& listener : listeners) {
        handled = listener->keyChange(window, code, press);
        if (handled) {
            break;
        }
    }
    return handled;
}