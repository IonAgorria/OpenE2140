//
// Created by Ion Agorria on 28/04/19
//
#ifndef OPENE2140_CAMERA_VIEW_H
#define OPENE2140_CAMERA_VIEW_H

#include "engine/core/macros.h"
#include "../gui_view.h"

class GUIGameRoot;

/**
 * Handles camera events
 */
class CameraView: public GUIView {
private:
    /**
     * Game root pointer
     */
    GUIGameRoot* gameRoot;

    /**
     * Camera current movement with keys
     */
    Vector2 keyMovement;

    /**
     * Camera current movement with mouse
     */
    Vector2 mouseMovement;

    /*
     * Key values
     */
    input_key_code_t keyUp = 0;
    input_key_code_t keyDown = 0;
    input_key_code_t keyLeft = 0;
    input_key_code_t keyRight = 0;
public:
    /**
     * Speed of camera when scrolling
     */
    int cameraSpeed = 10;

    /*
     * GUIView overrides
     */

    TYPE_NAME_OVERRIDE(CameraView);

    void layout() override;

    void update() override;

    bool mouseMove(int x, int y) override;

    bool keyChange(input_key_t& key) override;

    void rootChanged() override;
};

#endif //OPENE2140_CAMERA_VIEW_H
