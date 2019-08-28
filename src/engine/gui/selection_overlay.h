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

/**
 * Overlay for selection
 */
class SelectionOverlay: public Overlay {
protected:
    /**
     * Active selection rectangle
     */
    Rectangle rectangle;

    /** Color for neutral entities */
    ColorRGBA neutralColor = Color::WHITE;

    /** Color for own entities */
    ColorRGBA ownColor = Color::GREEN;

    /** Color for ally entities */
    ColorRGBA allyColor = Color::LIME;

    /** Color for enemy entities */
    ColorRGBA enemyColor = Color::RED;

public:
    /**
     * Current selected entities
     */
    std::set<std::shared_ptr<Entity>> selection;

    /**
     * Constructor
     */
    explicit SelectionOverlay(std::shared_ptr<Engine> engine);

    /**
     * Destructor
     */
    ~SelectionOverlay() override = default;

    void update() override;

    void draw(const Rectangle& viewport) override;

    /*
     * IEventListener overrides
     */

    bool eventMouseClick(Window* window, int x, int y, int button, bool press) override;
};

#endif //OPENE2140_SELECTION_OVERLAY_H
