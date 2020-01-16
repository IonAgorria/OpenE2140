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

bool SelectionOverlay::canSelect(const std::shared_ptr<Entity>& entity) {
    return entity->isActive() && entity->isSelectable();
}

void SelectionOverlay::rootChanged() {
    GUIView::rootChanged();
    gameRoot = dynamic_cast<GUIGameRoot*>(root);
    if (root && !gameRoot) {
        LOG_BUG("View root is not game root");
    }
}

void SelectionOverlay::update() {
    for (auto it = selection.begin(); it != selection.end(); ) {
        SelectionState& state = it->second;
        std::shared_ptr<Entity>& selected = state.entity;

        //Remove selection if certain conditions are met
        if (!canSelect(selected)) {
            it = selection.erase(it);
            continue;
        }

        //Move to next
        ++it;
    }

    //Calculate rectangle
    if (selectionStart && mousePosition) {
        Vector2 worldPosition = *mousePosition + gameRoot->getCamera();
        selectionRectangle.set(
                std::min(selectionStart->x, worldPosition.x),
                std::min(selectionStart->y, worldPosition.y),
                std::abs(selectionStart->x - worldPosition.x),
                std::abs(selectionStart->y - worldPosition.y)
        );
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

    //Check if there is a active rectangle and is big enough
    if (selectionStart
    && selectionRectangle.w > selectionThreshold
    && selectionRectangle.h > selectionThreshold) {
        renderer->drawRectangle(selectionRectangle, 1.0f, Color::WHITE);
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
    if (!canSelect(entity)) {
        return;
    }
    //Get player component, if missing then don't add this entity
    PlayerComponent* component = GET_COMPONENT_DYNAMIC(entity.get(), PlayerComponent);
    if (!component) {
        return;
    }

    //Handle player info
    Player* userPlayer = gameRoot->getUserPlayer();
    Player* player = component->getPlayer();
    ColorRGBA& color = neutralColor;
    if (!userPlayer || !player) {
        color = neutralColor;
    } else if (player == userPlayer) {
        color = ownColor;
    } else if (userPlayer->isEnemy(player)) {
        color = enemyColor;
    } else {
        color = allyColor;
    }

    selection[entity->getID()] = {entity, color};
}

void SelectionOverlay::removeEntity(entity_id_t id) {
    selection.erase(id);
}

bool SelectionOverlay::isSelected(entity_id_t id) {
    return selection.find(id) != selection.end();
}

bool SelectionOverlay::mouseClick(int x, int y, mouse_button_t button, bool press) {

    //Handle start of selection
    if (press && button == 1 && !selectionStart && mousePosition) {
        selectionStart = std::make_unique<Vector2>(*mousePosition + gameRoot->getCamera());
    }

    //Handle finish of rectangle
    if (!press && button == 1 && selectionStart) {
        auto engine = root->getEngine();
        bool isRectangle = selectionThreshold < selectionRectangle.w && selectionThreshold < selectionRectangle.h;

        //Remove previous entities if not in additive mode
        if (!additive) {
            selection.clear();
        }

        //Since only visible entities could have been selected we only check those
        for (auto& entity : gameRoot->visibleEntities) {
            //Handle according to selection rectangle or click mode
            bool select = isRectangle
                    ? selectionRectangle.isInside(entity->getPosition())
                    : entity->getBounds().isInside(*selectionStart);

            //Only add if was not previously selected
            if (select && !isSelected(entity->getID())) {
                engine->log->debug("Selected {0}", entity->toString());
                addEntity(entity);
            }
        }

        //Unset selection start
        selectionRectangle.set(0);
        selectionStart.reset();
    }

    return GUIView::mouseClick(x, y, button, press);
}

bool SelectionOverlay::keyChange(input_key_t& key) {
    additive = key.shift;
    return GUIView::keyChange(key);
}