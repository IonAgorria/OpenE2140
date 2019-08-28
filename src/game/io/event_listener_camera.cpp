//
// Created by Ion Agorria on 28/04/19
//

#include "engine/graphics/renderer.h"
#include "engine/math/vector2.h"
#include "game/core/constants.h"
#include "game/core/game.h"
#include "event_listener_camera.h"

EventListenerCamera::EventListenerCamera(const std::shared_ptr<Game>& game): game(game) {
    keyUp = game->getKeyBind("W");
    keyLeft = game->getKeyBind("A");
    keyDown = game->getKeyBind("S");
    keyRight = game->getKeyBind("D");
}

EventListenerCamera::~EventListenerCamera() {
    if (game) {
        game.reset();
    }
}

bool EventListenerCamera::eventUpdate() {
    //Nothing to do
    if (keyMovement.zero() && mouseMovement.zero()) return false;
    game->updateCamera(game->getCamera() + keyMovement + mouseMovement);
    return false;
}

bool EventListenerCamera::eventMouseMove(Window* window, int x, int y) {
    //TODO
    return false;
}

bool EventListenerCamera::eventKeyChange(Window* window, input_key_t& key) {
    if (key.press && !key.repeat) {
        if (key.code == keyUp) {
            keyMovement.y = -CAMERA_SPEED;
        } else if (key.code == keyLeft) {
            keyMovement.x = -CAMERA_SPEED;
        } else if (key.code == keyDown) {
            keyMovement.y =  CAMERA_SPEED;
        } else if (key.code == keyRight) {
            keyMovement.x =  CAMERA_SPEED;
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
    return false;
}