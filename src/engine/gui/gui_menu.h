//
// Created by Ion Agorria on 11/05/19
//
#ifndef OPENE2140_GUI_MENU_H
#define OPENE2140_GUI_MENU_H

#include "engine/io/event_listener.h"
#include "gui_view.h"

class Engine;

/**
 * A menu contains the different UI elements and current state one at a time including their transitions
 */
class GUIMenu: public GUIView, public IEventListener {
public:
    /**
     * Constructor
     */
    GUIMenu() = default;

    /**
     * Destructor
     */
    ~GUIMenu() override = default;

    /**
     * Disable copy
     */
    NON_COPYABLE(GUIMenu)

    /*
     * GUIView overrides
     */

    void update() override;

    /*
     * IEventListener overrides
     */

    bool eventMouseClick(Window* window, int x, int y, int button, bool press) override;

    bool eventMouseWheel(Window* window, int x, int y) override;

    bool eventMouseMove(Window* window, int x, int y) override;

    bool eventKeyChange(Window* window, input_key_t& key) override;
};

#endif //OPENE2140_GUI_MENU_H
