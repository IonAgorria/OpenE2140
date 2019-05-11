//
// Created by Ion Agorria on 28/04/19
//
#ifndef OPENE2140_UI_EVENT_LISTENER_H
#define OPENE2140_UI_EVENT_LISTENER_H

#include <src/engine/core/common.h>
#include <src/engine/io/event_handler.h>
#include "engine/io/event_listener.h"

class Game;
/**
 * Handles UI events
 */
class UIEventListener: public IEventListener {
    /**
     * Game object
     */
    std::shared_ptr<Game> game;

public:
    /**
     * Event handler constructor
     */
    UIEventListener(std::shared_ptr<Game> game);

    /**
     * Destroys event handler
     */
    virtual ~UIEventListener();

    /**
     * Disable copy/move
     */
    NON_COPYABLE(UIEventListener)

    bool mouseMove(Window* window, int x, int y) override;

    bool keyChange(Window* window, int code, bool press) override;
};

#endif //OPENE2140_UI_EVENT_LISTENER_H
