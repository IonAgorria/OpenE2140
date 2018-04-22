//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_WINDOW_H
#define OPENE2140_WINDOW_H

#include "SDL_video.h"
#include "SDL_render.h"
#include "core/io/log.h"
#include "image.h"

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
    /** Max texture size */
    Vector2 textureMaxSize;
public:
    /**
     * Creates the window with provided parameters
     *
     * @param width of window
     * @param height of window
     * @param title of window
     * @return if success
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
     * @return if window was created
     */
    operator bool();

    /**
     * Adds image to window in specified rectangle
     *
     * @param image to draw
     * @param rectangle to locate the image
     * @return if success
     */
    bool draw(Image& image, const Rectangle& rectangle);

    /**
     * Updates window and draws everything to screen
     *
     * @return if success
     */
    bool update();

    /**
     * Creates a texture for drawing in the window
     *
     * @param width size of image
     * @param height size of image
     * @param pixels content of image
     * @return image
     */
    texture_ptr createTexture(const int width, const int height);
};

#endif //OPENE2140_WINDOW_H
