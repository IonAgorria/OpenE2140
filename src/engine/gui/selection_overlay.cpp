//
// Created by Ion Agorria on 28/8/19
//

#include "engine/core/engine.h"
#include "engine/graphics/renderer.h"
#include "selection_overlay.h"

SelectionOverlay::SelectionOverlay(std::shared_ptr<Engine> engine) : Overlay(std::move(engine)) {

}

void SelectionOverlay::update() {
    for (auto it = selection.begin(); it != selection.end(); ) {
        std::shared_ptr<Entity> selected = *it;

        //Remove selection if certain conditions are met
        if (!selected->isActive() || selected->isDisable()) {
            it = selection.erase(it);
            continue;
        }

        //Move to next
        ++it;
    }
}

void SelectionOverlay::draw(const Rectangle& viewport) {
    Renderer* renderer = engine->getRenderer();
    for (const std::shared_ptr<Entity>& entity : selection) {
        const Rectangle& bounds = entity->getBounds();
        if (viewport.isOverlap(bounds)) {
            renderer->drawRectangle(bounds, 2, neutralColor);
        }
    }
}

bool SelectionOverlay::eventMouseClick(Window* window, int x, int y, int button, bool press) {
    return false;
}
