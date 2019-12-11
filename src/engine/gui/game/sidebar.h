//
// Created by Ion Agorria on 28/04/19
//
#ifndef OPENE2140_SIDEBAR_H
#define OPENE2140_SIDEBAR_H

#include "engine/core/macros.h"
#include "engine/gui/gui_view.h"

/**
 * Handles sidebar menu
 */
class Sidebar: public GUIView {
public:

    /*
     * GUIView overrides
     */

    TYPE_NAME_OVERRIDE(Sidebar);

    void update() override;

    void draw() override;
};

#endif //OPENE2140_SIDEBAR_H
