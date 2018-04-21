//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_WINDOW_H
#define OPENE2140_WINDOW_H

#include "SDL_video.h"
#include "SDL_render.h"
#include "log.h"

/**
 * Window class, each one contains a handle for a window.
 *
 * Window can be draw and receive input from OS
 */
class Window {
private:
    /** Log for object */
    log_ptr log;
    /** Window handle used for SDL2 */
    SDL_Window* windowHandle = nullptr;
    /** Renderer handle used for SDL2 */
    SDL_Renderer* rendererHandle = nullptr;

public:
    /**
     * Constructs a new window object
     */
    Window();

    /**
     * Destroys any created window
     */
    ~Window();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Window)

    /**
     * Creates the window with provided parameters
     *
     * @param width of window
     * @param height of window
     * @param title of window
     * @return if success
     */
    bool create(unsigned int width, uint height, const std::string& title);

    /**
     * Updates window
     *
     * @return if success
     */
    bool update();
};


#endif //OPENE2140_WINDOW_H
