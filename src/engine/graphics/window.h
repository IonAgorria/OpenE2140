//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_WINDOW_H
#define OPENE2140_WINDOW_H

#include <SDL_video.h>
#include <SDL_render.h>
#include "engine/core/common.h"
#include "engine/core/error_possible.h"
#include "engine/io/log.h"
#include "engine/math/vector2.h"

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
    window_id_t windowID = 0;

    /** Context used for SDL2 */
    SDL_GLContext context = nullptr;

    /** Current size */
    Vector2 size;
public:
    /**
     * Window constructor
     * Creates the window with provided parameters
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
    window_id_t getID();

    /**
     * @return current window size
     */
    const Vector2& getSize();

    /**
     * Updates the size to the current size
     *
     * @return current window size
     */
    const Vector2& updateSize();

    /**
     * Set current window size
     *
     * @param newSize to set
     * @return if succeed
     */
    bool setSize(const Vector2& newSize);

    /**
     * Set current window title
     *
     * @param title to set
     * @return if succeed
     */
    void setTitle(const std::string& title);

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
