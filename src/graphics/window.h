//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_WINDOW_H
#define OPENE2140_WINDOW_H

#include "SDL_video.h"
#include "SDL_render.h"
#include "io/log.h"
#include "image.h"

/**
 * Window listener
 */
class IWindowListener {
public:
    /**
     * IWindowListener destructor
     */
    virtual ~IWindowListener() = default;

    /**
     * Called when window is resized
     *
     * @param width of window
     * @param height of window
     */
    virtual void windowResize(int width, int height) = 0;

    /**
     * Called when mouse click occurs
     *
     * @param x position
     * @param y position
     * @param button pressed
     * @param press or release
     */
    virtual void mouseClick(int x, int y, int button, bool press) = 0;

    /**
     * Called when mouse movement occurs
     *
     * @param x position
     * @param y position
     */
    virtual void mouseMove(int x, int y) = 0;

    /**
     * Called when key change occurs
     *
     * @param code of key
     * @param name of key
     * @param press or release
     */
    virtual void keyChange(int code, std::string name, bool press) = 0;
};

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
    /** Context used for SDL2 */
    SDL_GLContext context = nullptr;
    /** Max texture size */
    int textureMaxSize;
    /** Window close state */
    bool closing;
public:
    /**
     * Window constructor
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
     * @return if window was created
     */
    operator bool();

    /**
     * Creates the window with provided parameters
     *
     * @param width of window
     * @param height of window
     * @param title of window
     */
    bool init(unsigned int width, unsigned int height, const std::string& title, IWindowListener& listener);

    /**
     * Polls input and events
     */
    void poll(IWindowListener& listener);

    /**
     * Updates window content
     *
     * @return if success
     */
    void swap();

    /**
     * @return if window must close
     */
    bool isClosing();
};

#endif //OPENE2140_WINDOW_H
