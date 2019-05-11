//
// Created by Ion Agorria on 27/04/19
//
#ifndef OPENE2140_EVENT_LISTENER_H
#define OPENE2140_EVENT_LISTENER_H

#include <string>

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
     * Called when window is changed (resized or made visible)
     *
     * @param window causing the event
     * @param width of window
     * @param height of window
     * @return true if handled to stop propagation
     */
    virtual bool windowChanged(Window* window);

    /**
     * Called when window focus is changed
     *
     * @param window causing the event
     * @param state current focus state
     * @return true if handled to stop propagation
     */
    virtual bool windowFocus(Window* window, bool state);

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
    virtual bool mouseClick(Window* window, int x, int y, int button, bool press);

    /**
     * Called when mouse wheel/scroll occurs
     *
     * @param window causing the event
     * @param x movement, toward left < 0 > toward right
     * @param y movement, toward user < 0 > away from the user
     * @return true if handled to stop propagation
     */
    virtual bool mouseWheel(Window* window, int x, int y);

    /**
     * Called when mouse movement occurs
     *
     * @param window causing the event
     * @param x position
     * @param y position
     * @return true if handled to stop propagation
     */
    virtual bool mouseMove(Window* window, int x, int y);

    /**
     * Called when key change occurs
     *
     * @param window causing the event
     * @param code of key
     * @param name of key
     * @param press or release
     * @return true if handled to stop propagation
     */
    virtual bool keyChange(Window* window, int code, bool press);
};

#endif //OPENE2140_EVENT_LISTENER_H
