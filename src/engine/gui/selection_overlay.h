//
// Created by Ion Agorria on 28/8/19
//
#ifndef OPENE2140_SELECTION_OVERLAY_H
#define OPENE2140_SELECTION_OVERLAY_H

#include <set>
#include "engine/graphics/color.h"
#include "engine/simulation/entity.h"
#include "overlay.h"

struct ColorRGBA;

struct SelectionState {
    std::shared_ptr<Entity> entity;
    ColorRGBA color;
};

/**
 * Overlay for selection
 */
class SelectionOverlay: public Overlay {
protected:
    /**
     * Active selection rectangle
     */
    Rectangle selectingRectangle;

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

public:
    /**
     * Constructor
     */
    explicit SelectionOverlay(std::shared_ptr<Engine> engine);

    /**
     * Destructor
     */
    ~SelectionOverlay() override = default;

    void update() override;

    void draw(const Rectangle& rectangle) override;

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
     * IEventListener overrides
     */

    bool eventMouseClick(Window* window, int x, int y, int button, bool press) override;
};

#endif //OPENE2140_SELECTION_OVERLAY_H
