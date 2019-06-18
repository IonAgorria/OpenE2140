//
// Created by Ion Agorria on 03/06/19
//
#ifndef OPENE2140_EVENT_LISTENER_DEBUG_H
#define OPENE2140_EVENT_LISTENER_DEBUG_H

#include <src/engine/core/common.h>
#include <src/engine/io/event_handler.h>
#include "engine/io/event_listener.h"

class Game;

/**
 * Handles debug events
 */
class EventListenerDebug: public IEventListener {
private:
    /**
     * Game object
     */
    std::shared_ptr<Game> game;

    /*
     * Key values
     */
    input_key_code_t keyAssetsReload = 0;
public:
    /**
     * Event handler constructor
     */
    EventListenerDebug(std::shared_ptr<Game> game);

    /**
     * Destroys event handler
     */
    virtual ~EventListenerDebug();

    /**
     * Disable copy
     */
    NON_COPYABLE(EventListenerDebug)

    bool keyChange(Window* window, input_key_t& key) override;
};

#endif //OPENE2140_EVENT_LISTENER_DEBUG_H
