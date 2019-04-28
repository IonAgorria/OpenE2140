//
// Created by Ion Agorria on 20/05/18
//
#include <SDL_events.h>
#include "src/engine/core/engine.h"
#include "graphics/renderer.h"
#include "graphics/window.h"
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
            case SDL_MOUSEMOTION: {
                mouseMove(window, event.motion.x, event.motion.y);
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                SDL_Keycode sym = event.key.keysym.sym;
                std::string name(SDL_GetKeyName(sym));
                keyChange(window, sym, name, event.key.state == SDL_PRESSED);
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
    log->debug("Window changed {0}x{1}", size.x, size.y);
    Renderer* renderer = engine->getRenderer();
    if (renderer) {
        renderer->changeViewport(size.x, size.y);
    }
    for (std::unique_ptr<IEventListener>& listener : listeners) {
        if (listener->windowChanged(window)) {
            break;
        }
    }
}

bool EventHandler::mouseClick(Window* window, int x, int y, int button, bool press) {
    log->debug("Mouse button: {0} at {1}x{2} {3}", button, x, y, press ? "press" : "release");
    for (std::unique_ptr<IEventListener>& listener : listeners) {
        if (listener->mouseClick(window, x, y, button, press)) {
            break;
        }
    }
}

bool EventHandler::mouseMove(Window* window, int x, int y) {
    //log->debug("Mouse motion: {0}x{1}", x, y);
    for (std::unique_ptr<IEventListener>& listener : listeners) {
        if (listener->mouseMove(window, x, y)) {
            break;
        }
    }
}

bool EventHandler::keyChange(Window* window, int code, const std::string& name, bool press) {
    log->debug("Key change: {0} '{1}' {2}", code, name, press ? "press" : "release");
    for (std::unique_ptr<IEventListener>& listener : listeners) {
        if (listener->keyChange(window, code, name, press)) {
            break;
        }
    }
}
