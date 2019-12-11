//
// Created by Ion Agorria on 11/05/19
//
#ifndef OPENE2140_GUI_ROOT_H
#define OPENE2140_GUI_ROOT_H

#include "engine/io/event_listener.h"
#include "gui_view.h"

class Engine;
class Simulation;

/**
 * A GUI root contains the different UI elements and current state one at a time including their transitions
 */
class GUIRoot: public virtual GUIView, public IEventListener {
protected:
    /**
     * Engine object
     */
    std::shared_ptr<Engine> engine;

    /**
     * Simulation pointer
     */
    Simulation* simulation = nullptr;

public:
    /**
     * Constructor
     */
    GUIRoot() = default;

    /**
     * Destructor
     */
    ~GUIRoot() override = default;

    /**
     * Disable copy
     */
    NON_COPYABLE(GUIRoot)

    /**
     * Type name
     */
    virtual TYPE_NAME_OVERRIDE(GUIRoot)

    /**
     * Called when this root is added or removed in engine as active root
     *
     * @param state
     */
    virtual void rootActive(std::shared_ptr<Engine> engine);

    /**
     * @return engine pointer
     */
    Engine* getEngine();

    /**
     * @return simulation pointer
     */
    Simulation* getSimulation();

    /*
     * GUIView overrides
     */

    void update() override;

    /*
     * IEventListener overrides
     */

    bool eventWindowChanged(Window* window) override;

    bool eventWindowFocus(Window* window, bool focus) override;

    bool eventMouseClick(Window* window, int x, int y, mouse_button_t button, bool press) override;

    bool eventMouseWheel(Window* window, int x, int y) override;

    bool eventMouseMove(Window* window, int x, int y) override;

    bool eventKeyChange(Window* window, input_key_t& key) override;
};

#endif //OPENE2140_GUI_ROOT_H
