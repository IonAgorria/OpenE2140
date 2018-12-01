//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_EVENTHANDLER_H
#define OPENE2140_EVENTHANDLER_H

#include <core/common.h>
#include <io/log.h>
#include <graphics/window.h>

class Game;

/**
 * Event handler to listen window events
 */
class EventHandler: public virtual IWindowListener {
    /** Log for object */
    log_ptr log;

    /**
     * Listener to send actions
     */
    std::shared_ptr<Game> game;
public:
    /**
     * Event handler constructor
     */
    EventHandler(std::shared_ptr<Game> game);

    /**
     * Destroys event handler
     */
    virtual ~EventHandler() override;

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
    virtual void windowResize(int width, int height) override;

    /**
     * Called when mouse click occurs
     *
     * @param x position
     * @param y position
     * @param button pressed
     * @param press or release
     */
    virtual void mouseClick(int x, int y, int button, bool press) override;

    /**
     * Called when mouse movement occurs
     *
     * @param x position
     * @param y position
     */
    virtual void mouseMove(int x, int y) override;

    /**
     * Called when key change occurs
     *
     * @param code of key
     * @param name of key
     * @param press or release
     */
    virtual void keyChange(int code, std::string name, bool press) override;
};


#endif //OPENE2140_EVENTHANDLER_H
