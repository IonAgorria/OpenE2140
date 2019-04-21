//
// Created by Ion Agorria on 20/05/18
//
#include <SDL_events.h>
#include "eventhandler.h"
#include "core/game.h"
#include "graphics/renderer.h"

EventHandler::EventHandler(std::shared_ptr<Game> game): game(game) {
    log = Log::get("EventHandler");
    closing = false;
}

EventHandler::~EventHandler() {
    log->debug("Closing");
    if (game) {
        game.reset();
    }
}

bool EventHandler::isClosing() {
    return closing;
}

void EventHandler::poll() {
    Window* window = game->getWindow();
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
                continue;
            }
            default: {
                continue;
            }
        }
    }
}

void EventHandler::windowChanged(Window* window) {
    Vector2 size = window->updateSize();
    log->debug("Window changed {0}x{1}", size.x, size.y);
    Renderer* renderer = game->getRenderer();
    if (renderer) {
        renderer->changeViewport(size.x, size.y);
    }
}

void EventHandler::mouseClick(Window* window, int x, int y, int button, bool press) {
    log->debug("Mouse button: {0} at {1}x{2} {3}", button, x, y, press ? "press" : "release");
}

void EventHandler::mouseMove(Window* window, int x, int y) {
    log->debug("Mouse motion: {0}x{1}", x, y);
}

void EventHandler::keyChange(Window* window, int code, std::string name, bool press) {
    log->debug("Key change: {0} '{1}' {2}", code, name, press ? "press" : "release");
    if (press) return;
    if (name == "Left") {
        game->test(-1);
    } else if (name == "Right") {
        game->test(1);
    }
}
