//
// Created by Ion Agorria on 28/04/19
//
#ifndef OPENE2140_EVENT_LISTENER_CAMERA_H
#define OPENE2140_EVENT_LISTENER_CAMERA_H

#include <src/engine/core/common.h>
#include <src/engine/io/event_handler.h>
#include "engine/io/event_listener.h"

class Game;

/**
 * Handles camera events
 */
class EventListenerCamera: public IEventListener {
private:
    /**
     * Game object
     */
    std::shared_ptr<Game> game;

    /**
     * Camera current movement with keys
     */
    Vector2 keyMovement;

    /**
     * Camera current movement with mouse
     */
    Vector2 mouseMovement;

    /*
     * Key values
     */
    unsigned int keyUp = 0;
    unsigned int keyDown = 0;
    unsigned int keyLeft = 0;
    unsigned int keyRight = 0;
public:
    /**
     * Event handler constructor
     */
    EventListenerCamera(std::shared_ptr<Game> game);

    /**
     * Destroys event handler
     */
    virtual ~EventListenerCamera();

    /**
     * Disable copy
     */
    NON_COPYABLE(EventListenerCamera)

    bool update() override;

    bool mouseMove(Window* window, int x, int y) override;

    bool keyChange(Window* window, input_key_t& key, bool press) override;
};

#endif //OPENE2140_EVENT_LISTENER_CAMERA_H
