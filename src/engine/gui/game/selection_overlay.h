//
// Created by Ion Agorria on 28/8/19
//
#ifndef OPENE2140_SELECTION_OVERLAY_H
#define OPENE2140_SELECTION_OVERLAY_H

#include <set>
#include "engine/graphics/color.h"
#include "engine/simulation/entity.h"
#include "src/engine/gui/gui_view.h"

class SimulationView;
struct ColorRGBA;

struct SelectionState {
    std::shared_ptr<Entity> entity;
    ColorRGBA color;
};

/**
 * Overlay for selection
 */
class SelectionOverlay: public GUIView {
protected:
    /**
     * Game root pointer
     */
    GUIGameRoot* gameRoot;

    /**
     * Active selection rectangle
     */
    std::unique_ptr<Rectangle> selectingRectangle;

    /**
     * Current selected entities
     */
    std::unordered_map<entity_id_t, SelectionState> selection;

    /** Color for neutral entities */
    ColorRGBA neutralColor = Color::WHITE;

    /** Color for own entities */
    ColorRGBA ownColor = Color::GREEN;

    /** Color for ally entities */
    ColorRGBA allyColor = Color::LIME;

    /** Color for enemy entities */
    ColorRGBA enemyColor = Color::RED;

    /** Flag for additive mode */
    bool additive = false;

    /**
     * Method that checks if entity can be selected, used for filtering unselectable entities
     * @param entity
     * @return
     */
    bool canSelect(const std::shared_ptr<Entity>& entity);

public:

    /**
     * Adds an entity to selection
     *
     * @param entity to add
     */
    void addEntity(const std::shared_ptr<Entity>& entity);

    /**
     * Removes an entity from selection
     *
     * @param id of entity
     */
    void removeEntity(entity_id_t id);

    /**
     * Returns if entity is already selected
     *
     * @param id of entity
     * @return if selected
     */
    bool isSelected(entity_id_t id);

    /*
     * GUIView overrides
     */

    void rootChanged() override;

    void layout() override;

    void update() override;

    void draw() override;

    /*
     * IEventListener overrides
     */

    bool mouseClick(int x, int y, mouse_button_t button, bool press) override;
};

#endif //OPENE2140_SELECTION_OVERLAY_H
