//
// Created by Ion Agorria on 22/03/18
//
#include <SDL_video.h>
#include "log.h"
#include "utils.h"
#include "window.h"

Window::Window() {
    log = Log::get(__func__);
}

Window::~Window() {
    log->debug("Window closing");
    if (rendererHandle != nullptr) {
        SDL_DestroyRenderer(rendererHandle);
        rendererHandle = nullptr;
    }
    if (windowHandle != nullptr) {
        SDL_DestroyWindow(windowHandle);
        windowHandle = nullptr;
    }
    Utils::checkSDLError(log);
}

bool Window::create(unsigned int width, unsigned int height, const std::string& title) {
    log->debug("Window get {0}x{1} title '{2}'", width, height, title);
    //Create window
    windowHandle = SDL_CreateWindow(
            title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL
    );
    if (!windowHandle) {
        Utils::showErrorDialog("SDL2 window is null: " + Utils::checkSDLError(), log, false);
        return false;
    }

    //Create renderer
    rendererHandle = SDL_CreateRenderer(windowHandle, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!rendererHandle) {
        Utils::showErrorDialog("SDL2 renderer is null: " + Utils::checkSDLError(), log, false);
        return false;
    }

    //Show window
    SDL_ShowWindow(windowHandle);
    return true;
}

bool Window::update() {
    if (SDL_RenderClear(rendererHandle) != 0) {
        Utils::showErrorDialog("SDL_RenderClear: " + Utils::checkSDLError(), log, false);
        return false;
    }
    //if (SDL_RenderCopy(rendererHandle, textureHandle, nullptr, nullptr) != 0) {
    //    Utils::showErrorDialog("SDL_RenderCopy: " + Utils::checkSDLError(), log, false);
    //    return false;
    //}
    SDL_RenderPresent(rendererHandle);
    return true;
}
