//
// Created by Ion Agorria on 28/8/19
//

#include "engine/core/engine.h"
#include "engine/io/log.h"
#include "engine/graphics/renderer.h"
#include "engine/simulation/components/player_component.h"
#include "selection_overlay.h"

SelectionOverlay::SelectionOverlay(std::shared_ptr<Engine> engine) : Overlay(std::move(engine)) {
}

void SelectionOverlay::update() {
    Player* userPlayer = engine->getUserPlayer();
    if (!userPlayer) {
        return;
    }

    for (auto it = selection.begin(); it != selection.end(); ) {
        SelectionState& state = it->second;
        std::shared_ptr<Entity>& selected = state.entity;

        //Remove selection if certain conditions are met
        if (!selected->isActive() || selected->isDisable()) {
            it = selection.erase(it);
            continue;
        }

        //Get component if any
        PlayerComponent* component = GET_COMPONENT(selected.get(), PlayerComponent);
        if (!component) {
            it = selection.erase(it);
            continue;
        }

        //Handle player info
        Player* player = component->getPlayer();
        if (!player) {
            state.color = neutralColor;
        } else if (player == userPlayer) {
            state.color = ownColor;
        } else if (userPlayer->isEnemy(player)) {
            state.color = enemyColor;
        } else {
            state.color = allyColor;
        }

        //Move to next
        ++it;
    }
}

void SelectionOverlay::draw(const Rectangle& rectangle) {
    Renderer* renderer = engine->getRenderer();
    for (const auto& pair : selection) {
        auto& state = pair.second;
        const Rectangle& bounds = state.entity->getBounds();
        if (rectangle.isOverlap(bounds)) {
            renderer->drawRectangle(bounds, 2, state.color);
        }
    }
}

void SelectionOverlay::addEntity(const std::shared_ptr<Entity>& entity) {
    selection[entity->getID()] = {entity, neutralColor};
}

void SelectionOverlay::removeEntity(entity_id_t id) {
    selection.erase(id);
}

bool SelectionOverlay::isSelected(entity_id_t id) {
    return selection.find(id) != selection.end();
}

bool SelectionOverlay::eventMouseClick(Window* window, int x, int y, int button, bool press) {
    //Only release
    if (press) return false;

    //Translate the position from window to world
    Vector2 position(x, y);
    position += engine->getCamera();

    //Since only visible entities could have been selected we only check those
    bool handled = false;
    for (auto& entity : engine->getVisibleEntities()) {
        //Get and check if click was inside bounds
        const Rectangle& bounds = entity->getBounds();
        if (bounds.isInside(position)) {
            engine->log->debug("Selected {0}", entity->toString());

            //Skip
            if (entity->isDisable()) {
                continue;
            }

            //Add if not selected, remove if not in additive mode
            if (!isSelected(entity->getID())) {
                addEntity(entity);
                return true;
            } else if (!additive) {
                removeEntity(entity->getID());
                return true;
            }
        }
    }

    return handled;
}
