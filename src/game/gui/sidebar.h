//
// Created by Ion Agorria on 28/04/19
//
#ifndef OPENE2140_SIDEBAR_H
#define OPENE2140_SIDEBAR_H

#include "engine/core/macros.h"
#include "engine/gui/gui_menu.h"

class Game;

/**
 * Handles sidebar menu
 */
class Sidebar: public GUIMenu {
private:
    /**
     * Game object
     */
    std::shared_ptr<Game> game;
public:

    /**
     * Event handler constructor
     */
    explicit Sidebar(std::shared_ptr<Game> game);

    /**
     * Destroys event handler
     */
    ~Sidebar() override;

    /**
     * Disable copy
     */
    NON_COPYABLE(Sidebar)

    void update() override;

    void draw() override;
};

#endif //OPENE2140_SIDEBAR_H
