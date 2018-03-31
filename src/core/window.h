//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_WINDOW_H
#define OPENE2140_WINDOW_H

#include <SDL.h>
#include "log.h"

class Window {
private:
    log_ptr log;
    std::string title;
    SDL_Window* windowHandle = nullptr;
    SDL_Renderer* rendererHandle = nullptr;
    SDL_Texture* textureHandle = nullptr;

public:
    Window();
    ~Window();
    bool create(unsigned int width, uint height, const char* title);
    bool update();
};


#endif //OPENE2140_WINDOW_H
