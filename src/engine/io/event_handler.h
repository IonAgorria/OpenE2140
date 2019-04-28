//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_EVENTHANDLER_H
#define OPENE2140_EVENTHANDLER_H

#include "core/common.h"
#include "io/log.h"
#include "event_dispatcher.h"

class Engine;

/**
 * Event handler to listen window events
 */
class EventHandler: public EventDispatcher {
    /** Log for object */
    log_ptr log;

    /**
     * Engine object
     */
    std::shared_ptr<Engine> engine;

    /** Close request state */
    bool closing;
public:
    /**
     * Event handler constructor
     */
    EventHandler(std::shared_ptr<Engine> engine);

    /**
     * Destroys event handler
     */
    virtual ~EventHandler();

    /**
     * Registers event listener
     *
     * @param listener to register
     */
    void addEventListener(std::unique_ptr<IEventListener> processor);

    /**
     * @return if close is requested
     */
    bool isClosing();

    /**
     * Polls input and events
     */
    void poll();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(EventHandler)

    bool windowChanged(Window* window) override;

    bool mouseClick(Window* window, int x, int y, int button, bool press) override;

    bool mouseMove(Window* window, int x, int y) override;

    bool keyChange(Window* window, int code, const std::string& name, bool press) override;
};


#endif //OPENE2140_EVENTHANDLER_H
