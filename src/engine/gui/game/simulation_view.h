//
// Created by Ion Agorria on 30/8/19
//
#ifndef OPENE2140_SIMULATION_VIEW_H
#define OPENE2140_SIMULATION_VIEW_H

#include <vector>
#include <memory>
#include "src/engine/gui/gui_view.h"

class Entity;
class GUIGameRoot;

/**
 * Basic view that provides a window to simulation
 */
class SimulationView: public GUIView {
protected:
    /**
     * Game root pointer
     */
    GUIGameRoot* gameRoot;

public:

    /*
     * GUIView overrides
     */

    TYPE_NAME_OVERRIDE(SimulationView);

    void draw() override;

    void rootChanged() override;
};

#endif //OPENE2140_SIMULATION_VIEW_H
