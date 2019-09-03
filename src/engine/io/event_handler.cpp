//
// Created by Ion Agorria on 20/05/18
//
#include <SDL_events.h>
#include "engine/core/engine.h"
#include "engine/graphics/renderer.h"
#include "engine/graphics/window.h"
#include "event_dispatcher.h"
#include "event_handler.h"

EventHandler::EventHandler(std::shared_ptr<Engine> engine): engine(std::move(engine)) {
    log = Log::get("EventHandler");
    closing = false;
}

EventHandler::~EventHandler() {
    log->debug("Closing");
    if (engine) {
        engine.reset();
    }
    listeners.clear();
}

bool EventHandler::isClosing() {
    return closing;
}

void EventHandler::poll() {
    Window* window = engine->getWindow();
    SDL_Event event;
    while (SDL_PollEvent(&event) == 1) {
        if (event.window.windowID && (!window || event.window.windowID != window->getID())) {
            //Event is for a window that is not current or window is not available
            continue;
        }

        //Handle event by type and subtype
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                eventMouseClick(
                        window,
                        event.button.x,
                        event.button.y,
                        event.button.button,
                        event.button.state == SDL_PRESSED
                );
                break;
            }
            case SDL_MOUSEWHEEL: {
                bool normal = event.wheel.direction == SDL_MOUSEWHEEL_NORMAL;
                eventMouseWheel(
                        window,
                        event.wheel.x * (normal ? 1 : -1),
                        event.wheel.y * (normal ? 1 : -1)
                );
                break;
            }
            case SDL_MOUSEMOTION: {
                eventMouseMove(
                        window,
                        event.motion.x,
                        event.motion.y
                );
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                input_key_t key;
                auto mod = event.key.keysym.mod;
                key.code = event.key.keysym.sym;
                key.press = event.key.state == SDL_PRESSED;
                key.repeat = event.key.repeat != 0;
                key.shift = (mod & KMOD_SHIFT) != 0;
                key.ctrl = (mod & KMOD_CTRL) != 0;
                key.meta = (mod & KMOD_GUI) != 0;
                key.alt = (mod & KMOD_ALT) != 0;
                eventKeyChange(window, key);
                break;
            }
            case SDL_WINDOWEVENT: {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_SHOWN: {
                        eventWindowChanged(window);
                        break;
                    }
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED: {
                        eventWindowChanged(window);
                        break;
                    }
                    case SDL_WINDOWEVENT_FOCUS_LOST: {
                        eventWindowFocus(window, false);
                        break;
                    }
                    case SDL_WINDOWEVENT_FOCUS_GAINED: {
                        eventWindowFocus(window, true);
                        break;
                    }
                    case SDL_WINDOWEVENT_CLOSE: {
                        //NOTE: Seems that MacOS uses this event instead of SDL_QUIT when window is requested to close
                        closing = true;
                        break;
                    }
                    default:
                        break;
                }
                break;
            }
            case SDL_QUIT: {
                closing = true;
                break;
            }
            default: {
                break;
            }
        }
    }
}

bool EventHandler::eventWindowChanged(Window* window) {
    Vector2 size = window->updateSize();
    log->debug("Window changed: {0}x{1}", size.x, size.y);

    //Update viewport
    Renderer* renderer = engine->getRenderer();
    if (renderer) {
        renderer->changeViewport(0, 0, size.x, size.y);
    }

    return EventDispatcher::eventWindowChanged(window);
}

bool EventHandler::eventWindowFocus(Window* window, bool state) {
    log->debug("Window focus: {0}", state ? "gained" : "lost");
    return EventDispatcher::eventWindowFocus(window, state);
}

bool EventHandler::eventMouseClick(Window* window, int x, int y, mouse_button_t button, bool press) {
    log->debug("Mouse button: {0} at {1}x{2} {3}", button, x, y, press ? "press" : "release");
    return EventDispatcher::eventMouseClick(window, x, y, button, press);
}

bool EventHandler::eventMouseWheel(Window* window, int x, int y) {
    log->debug("Mouse wheel: {0} {1}", x, y);
    return EventDispatcher::eventMouseWheel(window, x, y);
}

bool EventHandler::eventMouseMove(Window* window, int x, int y) {
    //log->debug("Mouse motion: {0}x{1}", x, y);
    return EventDispatcher::eventMouseMove(window, x, y);
}

bool EventHandler::eventKeyChange(Window* window, input_key_t& key) {
    log->debug("Key change: {0} '{1}' {2}", key.code, getNameFromCode(key.code), key.press ? "press" : "release");
    return EventDispatcher::eventKeyChange(window, key);
}

input_key_code_t EventHandler::getCodeFromName(const std::string& name) {
    SDL_Keycode code = SDL_GetKeyFromName(name.c_str());
    return code == SDLK_UNKNOWN ? 0 : code;
}

std::string EventHandler::getNameFromCode(const input_key_code_t code) {
    return std::string(SDL_GetKeyName(code));
}
