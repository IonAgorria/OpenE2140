//
// Created by Ion Agorria on 22/03/18
//
#include <SDL_events.h>
#include "core/common.h"
#include "core/utils.h"
#include "io/log.h"
#include "window.h"

Window::Window(unsigned int width, unsigned int height, const std::string& title) {
    log = Log::get("Window");

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

    //Get window ID
    windowID = SDL_GetWindowID(windowHandle);

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

    //Set parameters
    glClearColor(0.5, 0.5, 0.5, 1.0);

    //Check errors
    error = Utils::checkAnyError();
    if (!error.empty()) {
        return;
    }
}

Window::~Window() {
    log->debug("Closing");
    windowID = 0;
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

bool Window::show() {
    if (!check()) return false;
    SDL_ShowWindow(windowHandle);

    //Check errors
    error = Utils::checkAnyError();
    return !error.empty();
}

bool Window::hide() {
    if (!check()) return false;
    SDL_HideWindow(windowHandle);

    //Check errors
    error = Utils::checkAnyError();
    return !error.empty();
}

window_id Window::getID() {
    return windowID;
}

const Vector2& Window::getSize() {
    return size;
}

const Vector2& Window::updateSize() {
    int width = 0;
    int height = 0;
    SDL_GetWindowSize(windowHandle, &width, &height);
    size.set(width, height);
    return size;
}

bool Window::setSize(const Vector2& newSize) {
    if (!check()) return false;

    //Set size for vector and window
    size.set(newSize);
    SDL_SetWindowSize(windowHandle, size.x, size.y);

    //Check errors
    error = Utils::checkAnyError();
    return !error.empty();
}


bool Window::check() {
    return windowHandle != nullptr && context != nullptr;
}

void Window::clear() {
    if (this->check()) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void Window::swap() {
    SDL_GL_SwapWindow(windowHandle);
}