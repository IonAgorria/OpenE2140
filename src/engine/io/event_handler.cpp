//
// Created by Ion Agorria on 20/05/18
//
#include <SDL_events.h>
#include "src/engine/core/engine.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
#include "event_dispatcher.h"
#include "event_handler.h"

EventHandler::EventHandler(std::shared_ptr<Engine> engine): engine(engine) {
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

void EventHandler::addEventListener(std::unique_ptr<IEventListener> listener) {
    listeners.push_back(std::move(listener));
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
                mouseClick(
                        window,
                        event.button.x, event.button.y,
                        event.button.button,
                        event.button.state == SDL_PRESSED
                );
                break;
            }
            case SDL_MOUSEWHEEL: {
                bool normal = event.wheel.direction == SDL_MOUSEWHEEL_NORMAL;
                mouseWheel(
                        window,
                        event.wheel.x * (normal ? 1 : -1),
                        event.wheel.y * (normal ? 1 : -1)
                );
                break;
            }
            case SDL_MOUSEMOTION: {
                mouseMove(
                        window,
                        event.motion.x,
                        event.motion.y
                );
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                SDL_Keycode sym = event.key.keysym.sym;
                keyChange(window, sym, event.key.state == SDL_PRESSED);
                break;
            }
            case SDL_WINDOWEVENT: {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_SHOWN: {
                        windowChanged(window);
                        break;
                    }
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED: {
                        windowChanged(window);
                        break;
                    }
                    case SDL_WINDOWEVENT_FOCUS_LOST: {
                        windowFocus(window, false);
                        break;
                    }
                    case SDL_WINDOWEVENT_FOCUS_GAINED: {
                        windowFocus(window, true);
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

bool EventHandler::windowChanged(Window* window) {
    Vector2 size = window->updateSize();
    log->debug("Window changed: {0}x{1}", size.x, size.y);
    Renderer* renderer = engine->getRenderer();
    if (renderer) {
        renderer->changeViewport(0, 0, size.x, size.y);
    }
    return EventDispatcher::windowChanged(window);
}

bool EventHandler::windowFocus(Window* window, bool state) {
    log->debug("Window focus: {0}", state ? "gained" : "lost");
    return EventDispatcher::windowFocus(window, state);
}

bool EventHandler::mouseClick(Window* window, int x, int y, int button, bool press) {
    log->debug("Mouse button: {0} at {1}x{2} {3}", button, x, y, press ? "press" : "release");
    return EventDispatcher::mouseClick(window, x, y, button, press);
}

bool EventHandler::mouseWheel(Window* window, int x, int y) {
    log->debug("Mouse wheel: {0} {1}", x, y);
    return EventDispatcher::mouseWheel(window, x, y);
}

bool EventHandler::mouseMove(Window* window, int x, int y) {
    //log->debug("Mouse motion: {0}x{1}", x, y);
    return EventDispatcher::mouseMove(window, x, y);
}

bool EventHandler::keyChange(Window* window, int code, bool press) {
    log->debug("Key change: {0} '{1}' {2}", code, getNameFromCode(code), press ? "press" : "release");
    return EventDispatcher::keyChange(window, code, press);
}

int EventHandler::getCodeFromName(const std::string& name) {
    SDL_Keycode code = SDL_GetKeyFromName(name.c_str());
    return code == SDLK_UNKNOWN ? 0 : code;
}

std::string EventHandler::getNameFromCode(const int code) {
    return std::string(SDL_GetKeyName(code));
}
