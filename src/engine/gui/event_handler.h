//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_EVENTHANDLER_H
#define OPENE2140_EVENTHANDLER_H

#include "core/common.h"
#include "io/log.h"
#include "graphics/window.h"

class Game;

/**
 * Event handler to listen window events
 */
class EventHandler {
    /** Log for object */
    log_ptr log;

    /**
     * Listener to send actions
     */
    std::shared_ptr<Game> game;

    /** Close request state */
    bool closing;
public:
    /**
     * Event handler constructor
     */
    EventHandler(std::shared_ptr<Game> game);

    /**
     * Destroys event handler
     */
    virtual ~EventHandler();

    /**
     * @return if close is requested
     */
    bool isClosing();

    /**
     * Polls input and events
     */
    void poll();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(EventHandler)

    /**
     * Called when window is changed (resized or made visible)
     *
     * @param window causing the event
     * @param width of window
     * @param height of window
     */
    void windowChanged(Window* window);

    /**
     * Called when mouse click occurs
     *
     * @param window causing the event
     * @param x position
     * @param y position
     * @param button pressed
     * @param press or release
     */
    void mouseClick(Window* window, int x, int y, int button, bool press);

    /**
     * Called when mouse movement occurs
     *
     * @param window causing the event
     * @param x position
     * @param y position
     */
    void mouseMove(Window* window, int x, int y);

    /**
     * Called when key change occurs
     *
     * @param window causing the event
     * @param code of key
     * @param name of key
     * @param press or release
     */
    void keyChange(Window* window, int code, std::string name, bool press);
};


#endif //OPENE2140_EVENTHANDLER_H
