//
// Created by Ion Agorria on 28/04/19
//
#ifndef OPENE2140_EVENT_LISTENER_CAMERA_H
#define OPENE2140_EVENT_LISTENER_CAMERA_H

#include "engine/core/common.h"
#include "engine/io/event_handler.h"
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
    input_key_code_t keyUp = 0;
    input_key_code_t keyDown = 0;
    input_key_code_t keyLeft = 0;
    input_key_code_t keyRight = 0;
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

    bool eventMouseMove(Window* window, int x, int y) override;

    bool eventKeyChange(Window* window, input_key_t& key) override;
};

#endif //OPENE2140_EVENT_LISTENER_CAMERA_H
