//
// Created by Ion Agorria on 1/9/19
//
#ifndef OPENE2140_GUI_GAME_ROOT_H
#define OPENE2140_GUI_GAME_ROOT_H

#include "src/engine/gui/gui_root.h"

class Entity;
class Player;

/**
 * GUI root used for main gameplay
 */
class GUIGameRoot: public virtual GUIRoot {
protected:
    /**
     * Camera where player is looking
     */
    Vector2 camera;

    /**
     * Current player being controller by user
     */
    Player* userPlayer;

public:
    /**
     * Type name
     */
    TYPE_NAME_OVERRIDE(GUIGameRoot)

    /**
     * The entities drawn in last frame
     */
    std::vector<std::shared_ptr<Entity>> visibleEntities;

    /**
     * @return camera
     */
    const Vector2& getCamera();

    /**
     * Updates the player camera
     *
     * @param position to set
     */
    void setCamera(const Vector2& position);

    /**
     * Obtain the camera rectangle
     *
     * @param rectangle
     */
    void getCameraRectangle(Rectangle& rectangle);

    /**
     * @return player controlled by user if any
     */
    Player* getUserPlayer();

    /**
     * Set player controlled by user
     */
    void setUserPlayer(Player* player);

    /*
     * IEventListener overrides
     */

    bool eventWindowChanged(Window* window) override;
};

#endif //OPENE2140_GUI_GAME_ROOT_H
