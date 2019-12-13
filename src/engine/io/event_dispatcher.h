//
// Created by Ion Agorria on 28/04/19
//
#ifndef OPENE2140_EVENT_DISPATCHER_H
#define OPENE2140_EVENT_DISPATCHER_H

#include "event_listener.h"

class Window;
/**
 * Dispatches the received events to listeners
 */
class EventDispatcher: public IEventListener {
protected:
    /**
     * Listeners to send events
     */
    std::vector<std::shared_ptr<IEventListener>> listeners;
public:

    /**
     * Type name
     */
    TYPE_NAME_OVERRIDE(EventDispatcher)

    /**
     * Registers event listener
     *
     * @param listener to register
     */
    void addEventListener(std::shared_ptr<IEventListener> listener);

    /**
     * Removes event listener
     *
     * @param listener to remove
     */
    void removeEventListener(std::shared_ptr<IEventListener> listener);

    /*
     * IEventListener
     */

    bool eventUpdate() override;

    bool eventWindowChanged(Window* window) override;

    bool eventWindowFocus(Window* window, bool focus) override;

    bool eventMouseClick(Window* window, int x, int y, mouse_button_t button, bool press) override;

    bool eventMouseWheel(Window* window, int x, int y) override;

    bool eventMouseMove(Window* window, int x, int y) override;

    bool eventKeyChange(Window* window, input_key_t& key) override;

    /*
     * ITreePrintable
     */

    std::vector<const ITreePrintable*> getLeafs() const override;
};

#endif //OPENE2140_EVENT_DISPATCHER_H
