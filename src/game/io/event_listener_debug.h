//
// Created by Ion Agorria on 03/06/19
//
#ifndef OPENE2140_EVENT_LISTENER_DEBUG_H
#define OPENE2140_EVENT_LISTENER_DEBUG_H

#include "engine/core/macros.h"
#include "engine/io/event_handler.h"
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
    input_key_code_t keyDebugEntities = 0;
    input_key_code_t keyDebugTiles = 0;
    input_key_code_t keyDebugEvents = 0;
public:
    /**
     * Event handler constructor
     */
    explicit EventListenerDebug(const std::shared_ptr<Game>& game);

    /**
     * Destroys event handler
     */
    ~EventListenerDebug() override;

    /**
     * Disable copy
     */
    NON_COPYABLE(EventListenerDebug)

    /**
     * Type name
     */
    TYPE_NAME_OVERRIDE(EventListenerDebug)

    bool eventKeyChange(Window* window, input_key_t& key) override;
};

#endif //OPENE2140_EVENT_LISTENER_DEBUG_H
