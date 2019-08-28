//
// Created by Ion Agorria on 28/04/19
//

#include "engine/graphics/renderer.h"
#include "game/core/constants.h"
#include "game/core/game.h"
#include "sidebar.h"

#include <utility>

Sidebar::Sidebar(std::shared_ptr<Game> game): game(std::move(game)) {
}

Sidebar::~Sidebar() {
    if (game) {
        game.reset();
    }
}

void Sidebar::update() {
    GUIMenu::update();
}

void Sidebar::draw() {
    GUIMenu::draw();
}