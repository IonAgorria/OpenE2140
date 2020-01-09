//
// Created by Ion Agorria on 28/04/19
//

#include "engine/graphics/renderer.h"
#include "engine/core/engine.h"
#include "camera_view.h"
#include "gui_game_root.h"

void CameraView::rootChanged() {
    GUIView::rootChanged();

    gameRoot = dynamic_cast<GUIGameRoot*>(root);
    if (root && !gameRoot) {
        LOG_BUG("View root is not game root");
    }

    if (!root) return;
    auto engine = root->getEngine();
    keyUp = engine->getKeyBind("W");
    keyLeft = engine->getKeyBind("A");
    keyDown = engine->getKeyBind("S");
    keyRight = engine->getKeyBind("D");
}

void CameraView::layout() {
    //Copy the rectangle of parent
    if (parent) {
        Rectangle rectangle;
        rectangle.setSize(parent->getRectangle());
        setRectangle(rectangle);
    }
    GUIView::layout();
}

void CameraView::update() {
    if (!keyMovement.zero() || !mouseMovement.zero()) {
        gameRoot->setCamera(gameRoot->getCamera() + keyMovement + mouseMovement);
    }

    GUIView::update();
}

bool CameraView::mouseMove(int x, int y) {
    //TODO
    return GUIView::mouseMove(x, y);
}

bool CameraView::keyChange(input_key_t& key) {
    if (key.press && !key.repeat) {
        if (key.code == keyUp) {
            keyMovement.y = -cameraSpeed;
        } else if (key.code == keyLeft) {
            keyMovement.x = -cameraSpeed;
        } else if (key.code == keyDown) {
            keyMovement.y =  cameraSpeed;
        } else if (key.code == keyRight) {
            keyMovement.x =  cameraSpeed;
        }
    } else if (!key.press) {
        //Only unset if keyMovement matches the correct value
        if (key.code == keyUp && keyMovement.y < 0) {
            keyMovement.y = 0;
        } else if (key.code == keyLeft && keyMovement.x < 0) {
            keyMovement.x = 0;
        } else if (key.code == keyDown && keyMovement.y > 0) {
            keyMovement.y = 0;
        } else if (key.code == keyRight && keyMovement.x > 0) {
            keyMovement.x = 0;
        }
    }

    return GUIView::keyChange(key);
}