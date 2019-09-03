//
// Created by Ion Agorria on 28/8/19
//

#include "engine/core/engine.h"
#include "engine/io/log.h"
#include "engine/graphics/renderer.h"
#include "engine/simulation/components/player_component.h"
#include "src/engine/gui/gui_root.h"
#include "gui_game_root.h"
#include "selection_overlay.h"

void SelectionOverlay::rootChanged() {
    GUIView::rootChanged();
    gameRoot = dynamic_cast<GUIGameRoot*>(root);
    if (root && !gameRoot) {
        BUG("View root is not game root");
    }
}

void SelectionOverlay::update() {
    Player* userPlayer = gameRoot->getUserPlayer();
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
        if (!userPlayer || !player) {
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

    GUIView::update();
}

void SelectionOverlay::draw() {
    //Get the camera rectangle
    Rectangle cameraRectangle;
    gameRoot->getCameraRectangle(cameraRectangle);

    for (const auto& pair : selection) {
        auto& state = pair.second;
        const Rectangle& bounds = state.entity->getBounds();

        //Check if visible
        if (cameraRectangle.isOverlap(bounds)) {
            renderer->drawRectangle(bounds, 1.0f, state.color);
        }
    }

    GUIView::draw();
}

void SelectionOverlay::layout() {
    //Copy the rectangle of parent
    if (parent) {
        Rectangle rectangle;
        rectangle.setSize(parent->getRectangle());
        setRectangle(rectangle);
    }
    GUIView::layout();
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

bool SelectionOverlay::mouseClick(int x, int y, mouse_button_t button, bool press) {
    //Only release
    if (!press && mousePosition) {
        auto engine = root->getEngine();

        //Translate the position from window to world
        Vector2 worldPosition = *mousePosition + gameRoot->getCamera();

        //Since only visible entities could have been selected we only check those
        for (auto& entity : gameRoot->visibleEntities) {
            //Get and check if click was inside bounds
            const Rectangle& bounds = entity->getBounds();
            if (bounds.isInside(worldPosition)) {
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
    }

    return GUIView::mouseClick(x, y, button, press);
}