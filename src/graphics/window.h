//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_WINDOW_H
#define OPENE2140_WINDOW_H

#include <SDL_video.h>
#include <SDL_render.h>
#include "core/errorpossible.h"
#include "io/log.h"
#include "image.h"

/**
 * Window class, each one contains a handle for a window.
 *
 * Window can be draw and receive input from OS
 */
class Window: public IErrorPossible {
private:
    /** Log for object */
    log_ptr log;

    /** Window handle used for SDL2 */
    SDL_Window* windowHandle = nullptr;

    /** Window ID used for SDL2 */
    unsigned int windowID = 0;

    /** Context used for SDL2 */
    SDL_GLContext context = nullptr;
public:
    /**
     * Window constructor
     * Creates the window with provided parameters
     *
     * @param width of window
     * @param height of window
     * @param title of window
     */
    Window(unsigned int width, unsigned int height, const std::string& title);

    /**
     * Destroys any created window
     */
    ~Window();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Window)

    /**
     * Shows the window
     * @return if succeed
     */
    bool show();

    /**
     * Hides the window
     * @return if succeed
     */
    bool hide();

    /**
     * @return window id
     */
    unsigned int getID();

    /**
     * @return current window size
     */
    Vector2 getSize();

    /**
     * Set current window size
     * @return if succeed
     */
    bool setSize(Vector2 size);

    /**
     * @return if window was created
     */
    bool check();

    /**
     * Clears window content
     */
    void clear();

    /**
     * Updates window content
     */
    void swap();
};

#endif //OPENE2140_WINDOW_H
