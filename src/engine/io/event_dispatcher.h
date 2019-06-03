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
    std::vector<std::unique_ptr<IEventListener>> listeners;
public:
    bool update() override;

    bool windowChanged(Window* window) override;

    bool windowFocus(Window* window, bool focus) override;

    bool mouseClick(Window* window, int x, int y, int button, bool press) override;

    bool mouseWheel(Window* window, int x, int y) override;

    bool mouseMove(Window* window, int x, int y) override;

    bool keyChange(Window* window, input_key_t& key) override;
};

#endif //OPENE2140_EVENT_DISPATCHER_H
