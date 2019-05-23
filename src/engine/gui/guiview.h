//
// Created by Ion Agorria on 11/05/19
//
#ifndef OPENE2140_GUIVIEW_H
#define OPENE2140_GUIVIEW_H

#include "math/rectangle.h"
#include "core/common.h"

/**
 * Basic element for composing GUI
 */
class GUIView {
protected:
    /**
     * View rectangle
     */
    Rectangle rectangle;

    /**
     * Parent view for this view
     */
    GUIView* parent;

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
    const std::vector<std::unique_ptr<GUIView>> getViews() const;

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
    virtual void moved(GUIView* newParent);

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
    virtual bool mouseClick(int x, int y, int button, bool press);

    /**
     * Called when mouse wheel/scroll occurs
     *
     * @param x movement, toward left < 0 > toward right
     * @param y movement, toward user < 0 > away from the user
     * @return true if handled to stop propagation
     */
    virtual bool mouseWheel(int x, int y);

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
    virtual bool keyChange(int code, bool press);
};

#endif //OPENE2140_GUIVIEW_H
