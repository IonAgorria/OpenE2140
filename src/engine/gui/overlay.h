//
// Created by Ion Agorria on 28/8/19
//
#ifndef OPENE2140_OVERLAY_H
#define OPENE2140_OVERLAY_H

#include <src/engine/math/rectangle.h>
#include "engine/core/macros.h"
#include "engine/io/event_listener.h"

class Engine;

/**
 * Implements a overlay for drawing on top of simulation draw
 */
class Overlay: public IEventListener {
protected:
    /**
     * Engine object
     */
    std::shared_ptr<Engine> engine;

public:
    /**
     * Constructor
     */
    explicit Overlay(std::shared_ptr<Engine> engine): engine(std::move(engine)) {}

    /**
     * Destructor
     */
    ~Overlay() override = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Overlay)

    virtual void update() = 0;

    virtual void draw(const Rectangle& viewport) = 0;
};

#endif //OPENE2140_OVERLAY_H
