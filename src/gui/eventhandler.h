//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_EVENTHANDLER_H
#define OPENE2140_EVENTHANDLER_H

#include <core/config.h>
#include <io/log.h>
#include <graphics/window.h>

/**
 * Event handler to listen window events
 */
class Game;
class EventHandler: public WindowListener {
    /** Log for object */
    log_ptr log;

    /**
     * Listener to send actions
     */
    Game& game;
public:
    /**
     * Event handler constructor
     */
    EventHandler(Game& game);

    /**
     * Destroys event handler
     */
    virtual ~EventHandler() = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(EventHandler)

    /**
     * Called when window is resized
     *
     * @param width of window
     * @param height of window
     */
    virtual void windowResize(int width, int height);

    /**
     * Called when mouse click occurs
     *
     * @param x position
     * @param y position
     * @param button pressed
     * @param press or release
     */
    virtual void mouseClick(int x, int y, int button, bool press);

    /**
     * Called when mouse movement occurs
     *
     * @param x position
     * @param y position
     */
    virtual void mouseMove(int x, int y);

    /**
     * Called when key change occurs
     *
     * @param code of key
     * @param name of key
     * @param press or release
     */
    virtual void keyChange(int code, std::string name, bool press);
};


#endif //OPENE2140_EVENTHANDLER_H
