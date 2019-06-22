//
// Created by Ion Agorria on 27/04/19
//
#ifndef OPENE2140_EVENT_LISTENER_H
#define OPENE2140_EVENT_LISTENER_H

#include <string>
#include "engine/core/types.h"

class Window;

/**
 * Event listener methods
 */
class IEventListener {
public:
    /**
     * Destructor
     */
    virtual ~IEventListener() = default;

    /**
     * Called when event listener is updated
     *
     * @return true if handled to stop propagation
     */
    virtual bool update();

    /**
     * Called when window is changed (resized or made visible)
     *
     * @param window causing the event
     * @param width of window
     * @param height of window
     * @return true if handled to stop propagation
     */
    virtual bool eventWindowChanged(Window* window);

    /**
     * Called when window focus is changed
     *
     * @param window causing the event
     * @param state current focus state
     * @return true if handled to stop propagation
     */
    virtual bool eventWindowFocus(Window* window, bool state);

    /**
     * Called when mouse click occurs
     *
     * @param window causing the event
     * @param x position
     * @param y position
     * @param button pressed
     * @param press or release
     * @return true if handled to stop propagation
     */
    virtual bool eventMouseClick(Window* window, int x, int y, int button, bool press);

    /**
     * Called when mouse wheel/scroll occurs
     *
     * @param window causing the event
     * @param x movement, toward left < 0 > toward right
     * @param y movement, toward user < 0 > away from the user
     * @return true if handled to stop propagation
     */
    virtual bool eventMouseWheel(Window* window, int x, int y);

    /**
     * Called when mouse movement occurs
     *
     * @param window causing the event
     * @param x position
     * @param y position
     * @return true if handled to stop propagation
     */
    virtual bool eventMouseMove(Window* window, int x, int y);

    /**
     * Called when key change occurs
     *
     * @param window causing the event
     * @param key data
     * @param name of key
     * @param press or release
     * @return true if handled to stop propagation
     */
    virtual bool eventKeyChange(Window* window, input_key_t& key);
};

#endif //OPENE2140_EVENT_LISTENER_H
