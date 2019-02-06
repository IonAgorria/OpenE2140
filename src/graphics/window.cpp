//
// Created by Ion Agorria on 22/03/18
//
#include "core/common.h"
#include "core/utils.h"
#include <SDL_events.h>
#include "io/log.h"
#include "window.h"

Window::Window(unsigned int width, unsigned int height, const std::string& title, IWindowListener& listener) {
    log = Log::get("Window");
    closing = false;

    //Create window
    log->debug("Creating window {0}x{1} title '{2}'", width, height, title);
    windowHandle = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );

    //Check errors
    error = Utils::checkSDLError();
    if (!windowHandle) {
        error = "SDL2 window not available\n" + error;
    }
    if (!error.empty()) return;

    //Set some attributes before context creation
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    //Use OpenGL 3.2 Core
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    //Create OpenGL context
    context = SDL_GL_CreateContext(windowHandle);
    error = Utils::checkAnyError();
    if (!context) {
        error = "SDL OpenGL context not available\n" + error;
    }
    if (!error.empty()) return;

    //Initialize GLEW
    glewExperimental = GL_TRUE; //required for "new" OpenGL
    GLenum glewResult = glewInit();

    //Check errors
    error = Utils::checkAnyError();
    if (glewResult != GLEW_OK) {
        std::string glewResultString = std::string(reinterpret_cast<const char*>(glewGetErrorString(glewResult)));
        error = "GLEW init failed\n" + glewResultString + "\n" + error;
    }
    if (!error.empty()) return;

    //Print some strings related to GL
    log->debug("GL_VERSION: {0}", glGetString(GL_VERSION));
    log->debug("GL_VENDOR: {0}", glGetString(GL_VENDOR));
    log->debug("GL_RENDERER: {0}", glGetString(GL_RENDERER));
    log->debug("GL_SHADING_LANGUAGE_VERSION: {0}", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int value = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
    log->debug("GL_CONTEXT_MAJOR_VERSION: {0}", value);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
    log->debug("GL_CONTEXT_MINOR_VERSION: {0}", value);

    //Check the texture size
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &textureMaxSize);
    log->debug("GL_MAX_TEXTURE_SIZE: {0}", textureMaxSize);
    if (textureMaxSize < MINIMUM_TEXTURE_SIZE) {
        error = "Maximum texture size is too small: " + std::to_string(textureMaxSize);
        return;
    }

    //Set parameters
    glClearColor(0.5, 0.5, 0.5, 1.0);

    //Check errors
    error = Utils::checkAnyError();
    if (!error.empty()) {
        return;
    }

    //Send the initial resize event
    listener.windowResize(width, height);

    //Show window
    SDL_ShowWindow(windowHandle);

    //Check errors
    error = Utils::checkAnyError();
    if (!error.empty()) {
        return;
    }
}

Window::~Window() {
    log->debug("Closing");
    if (context != nullptr) {
        SDL_GL_DeleteContext(context);
        context = nullptr;
    }
    Utils::checkGLError(log);
    if (windowHandle != nullptr) {
        SDL_DestroyWindow(windowHandle);
        windowHandle = nullptr;
    }
    Utils::checkSDLError(log);
}

Window::operator bool() {
    return windowHandle != nullptr && context != nullptr;
}

void Window::poll(IWindowListener& listener) {
    //Handle any events
    SDL_Event event;
    while (SDL_PollEvent(&event) == 1) {
        switch (event.type) {
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                listener.mouseClick(
                        event.button.x, event.button.y,
                        event.button.button,
                        event.button.state == SDL_PRESSED
                );
                break;
            }
            case SDL_MOUSEMOTION: {
                listener.mouseMove(event.motion.x, event.motion.y);
                break;
            }
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                SDL_Keycode sym = event.key.keysym.sym;
                std::string name(SDL_GetKeyName(sym));
                listener.keyChange(sym, name, event.key.state == SDL_PRESSED);
                break;
            }
            case SDL_WINDOWEVENT: {
                switch (event.window.event) {
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        listener.windowResize(event.window.data1, event.window.data2);
                        break;
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

void Window::swap() {
    SDL_GL_SwapWindow(windowHandle);
    glClear(GL_COLOR_BUFFER_BIT);
}

bool Window::isClosing() {
    return closing;
}
