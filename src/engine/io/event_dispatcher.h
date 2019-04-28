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
    bool windowChanged(Window* window) override;

    bool mouseClick(Window* window, int x, int y, int button, bool press) override;

    bool mouseMove(Window* window, int x, int y) override;

    bool keyChange(Window* window, int code, const std::string& name, bool press) override;
};

#endif //OPENE2140_EVENT_DISPATCHER_H
