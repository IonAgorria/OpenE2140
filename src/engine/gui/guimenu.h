//
// Created by Ion Agorria on 11/05/19
//
#ifndef OPENE2140_GUIMENU_H
#define OPENE2140_GUIMENU_H

#include "io/event_listener.h"
#include "guiview.h"

/**
 * A menu contains the different UI elements and current state one at a time including their transitions
 */
class GUIMenu: public GUIView, public IEventListener {
private:
    /**
     * Engine object
     */
    std::shared_ptr<Engine> engine;

public:
    /**
     * Constructor
     */
    GUIMenu(std::shared_ptr<Engine> engine);

    /**
     * Destructor
     */
    virtual ~GUIMenu();

    /**
     * Disable copy
     */
    NON_COPYABLE(GUIMenu)

    /*
     * IEventListener overrides
     */

    bool eventMouseClick(Window* window, int x, int y, int button, bool press) override;

    bool eventMouseWheel(Window* window, int x, int y) override;

    bool eventMouseMove(Window* window, int x, int y) override;

    bool eventKeyChange(Window* window, input_key_t& key) override;
};

#endif //OPENE2140_GUIMENU_H
