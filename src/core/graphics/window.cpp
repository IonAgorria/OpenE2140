//
// Created by Ion Agorria on 22/03/18
//
#include "core/io/log.h"
#include "core/utils.h"
#include "window.h"

Window::Window(unsigned int width, unsigned int height, const std::string& title) {
    log = Log::get(__func__);
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
        Utils::showErrorDialog("SDL2 window is null " + Utils::checkSDLError(), log, false);
        return;
    }

    //Create renderer
    rendererHandle = SDL_CreateRenderer(windowHandle, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!rendererHandle) {
        Utils::showErrorDialog("SDL2 renderer is null " + Utils::checkSDLError(), log, false);
        return;
    }

    //Show window
    SDL_ShowWindow(windowHandle);
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

Window::operator bool() {
    return windowHandle != nullptr && rendererHandle != nullptr;
}

bool Window::draw(Image& image, const Rectangle& rectangle) {
    WindowTexture texture = image;
    if (SDL_RenderCopy(rendererHandle, texture, &image.getRectangle(), &rectangle) != 0) {
        Utils::showErrorDialog("SDL_RenderCopy error when drawing " + Utils::checkSDLError(), log, false);
        return false;
    }
    return true;
}

bool Window::update() {
    //Show the current rendered
    SDL_RenderPresent(rendererHandle);

    //Clear for next iteration
    if (SDL_RenderClear(rendererHandle) != 0) {
        Utils::showErrorDialog("SDL_RenderClear " + Utils::checkSDLError(), log, false);
        return false;
    }
    return true;
}

std::shared_ptr<Texture> Window::createTexture(const int width, const int height) {
    std::shared_ptr<Texture> texture;
    WindowTexture windowTexture = SDL_CreateTexture(rendererHandle, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
    if (!windowTexture) {
        Utils::showErrorDialog("SDL_CreateTexture texture is not valid " + Utils::checkSDLError(), log, false);
    } else {
        texture = std::make_shared<Texture>(windowTexture, width, height);
    }
    return texture;
}
