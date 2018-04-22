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
        Utils::showErrorDialog("SDL2 window not available\n" + Utils::checkSDLError(), log, false);
        return;
    }

    //Create renderer
    rendererHandle = SDL_CreateRenderer(windowHandle, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!rendererHandle) {
        Utils::showErrorDialog("SDL2 renderer not available\n" + Utils::checkSDLError(), log, false);
        return;
    }

    //Fetch renderer info
    textureMaxSize = Vector2(MINIMUM_TEXTURE_SIZE, MINIMUM_TEXTURE_SIZE); //Assume default as minimum
    SDL_RendererInfo rendererInfo;
    std::string rendererName = "Unknown";
    if (SDL_GetRendererInfo(rendererHandle, &rendererInfo) == 0) {
        rendererName = rendererInfo.name;
        textureMaxSize.x = rendererInfo.max_texture_width;
        textureMaxSize.y = rendererInfo.max_texture_height;
    }
    log->debug("Using renderer: {0}", rendererName);
    log->debug("Maximum texture size: {0}", textureMaxSize.toString());

    //Check the texture size
    if (textureMaxSize.x < MINIMUM_TEXTURE_SIZE || textureMaxSize.y < MINIMUM_TEXTURE_SIZE) {
        Utils::showErrorDialog("Texture size is too small: " + textureMaxSize.toString() + "\nRenderer: " + rendererName, log, false);
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
    texture_ptr texture = image;
    if (SDL_RenderCopy(rendererHandle, texture.get(), &image.getRectangle(), &rectangle) != 0) {
        Utils::showErrorDialog("SDL_RenderCopy error when drawing\n" + Utils::checkSDLError(), log, false);
        return false;
    }
    return true;
}

bool Window::update() {
    //Show the current rendered
    SDL_RenderPresent(rendererHandle);

    //Clear for next iteration
    if (SDL_RenderClear(rendererHandle) != 0) {
        Utils::showErrorDialog("SDL_RenderClear error when clearing\n" + Utils::checkSDLError(), log, false);
        return false;
    }
    return true;
}

texture_ptr Window::createTexture(const int width, const int height) {
    texture_ptr texture;
    SDL_Texture* textureSDL = SDL_CreateTexture(rendererHandle, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
    if (!textureSDL) {
        Utils::showErrorDialog("SDL_CreateTexture texture is not valid\n" + Utils::checkSDLError(), log, false);
    } else {
        //Set pointer to texture and deleter for texture
        texture.reset(textureSDL, SDL_DestroyTexture);
    }
    return texture;
}
