//
// Created by Ion Agorria on 11/05/19
//
#ifndef OPENE2140_GUI_VIEW_H
#define OPENE2140_GUI_VIEW_H

#include <memory>
#include <vector>
#include "engine/math/rectangle.h"
#include "engine/core/macros.h"
#include "engine/core/types.h"

class GUIRoot;
class Renderer;

/**
 * Basic element for composing GUI
 */
class GUIView {
protected:
    /**
     * View rectangle in parent relative coordinates
     */
    Rectangle rectangle;

    /**
     * View rectangle in root coordinates
     */
    Vector2 rootPosition;

    /**
     * Current pressing mouse button
     */
    mouse_button_t mousePressingButton = 0;

    /**
     * Mouse position inside the view if any
     * It's also used to know if mouse is inside view
     */
    std::unique_ptr<Vector2> mousePosition;

    /**
     * Renderer to use when drawing this view
     */
    Renderer* renderer = nullptr;

    /**
     * Root view for this view
     */
    GUIRoot* root = nullptr;

    /**
     * Parent view for this view
     */
    GUIView* parent = nullptr;

    /**
     * Views inside this view
     */
    std::vector<std::unique_ptr<GUIView>> views;

public:
    /**
     * Constructor
     */
    GUIView();

    /**
     * Destructor
     */
    virtual ~GUIView();

    /**
     * Disable copy
     */
    NON_COPYABLE(GUIView)

    /**
     * Return the view views
     */
    const std::vector<std::unique_ptr<GUIView>>& getViews() const;

    /**
     * Adds view to this view
     */
    virtual void addView(std::unique_ptr<GUIView> view);

    /**
     * Removes view from this view
     */
    virtual std::unique_ptr<GUIView> removeView(GUIView* view);

    /**
     * Removes all views from this view
     */
    virtual void removeViews();

    /**
     * Called when view parent is changed
     */
    virtual void parentChanged(GUIView* newParent);

    /**
     * Called when root is changed
     */
    virtual void rootChanged();

    /**
     * @return view rectangle
     */
    const Rectangle& getRectangle() const;

    /**
     * Sets view rectangle
     *
     * @param newRectangle
     */
    void setRectangle(const Rectangle& newRectangle);

    /**
     * Called when view and subviews layout should be updated
     */
    virtual void layout();

    /**
     * Called when view is show
     */
    virtual void show();

    /**
     * Called when view is no longer visible
     */
    virtual void hide();

    /**
     * Called when view is being updated
     */
    virtual void update();

    /**
     * Draws this view
     */
    virtual void draw();

    /**
     * Called when mouse click occurs
     *
     * @param x position
     * @param y position
     * @param button pressed
     * @param press or release
     * @return true if handled to stop propagation
     */
    virtual bool mouseClick(int x, int y, mouse_button_t button, bool press);

    /**
     * Called when mouse wheel/scroll occurs
     *
     * @param x movement, toward left < 0 > toward right
     * @param y movement, toward user < 0 > away from the user
     * @return true if handled to stop propagation
     */
    virtual bool mouseWheel(int x, int y);

    /**
     * Called when mouse enters or leaves view
     *
     * @param state true if inside
     */
    virtual void mouseOver(bool state);

    /**
     * Called when mouse movement occurs
     *
     * @param x position
     * @param y position
     * @return true if handled to stop propagation
     */
    virtual bool mouseMove(int x, int y);

    /**
     * Called when key change occurs
     *
     * @param code of key
     * @param name of key
     * @param press or release
     * @return true if handled to stop propagation
     */
    virtual bool keyChange(input_key_t& key);
};

#endif //OPENE2140_GUI_VIEW_H
