//
// Created by Ion Agorria on 1/9/19
//
#include "engine/graphics/renderer.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/world/world.h"
#include "engine/simulation/player.h"
#include "gui_game_root.h"

const Vector2& GUIGameRoot::getCamera() {
    return camera;
}

void GUIGameRoot::setCamera(const Vector2& position) {
    this->camera.set(position);

    //Limit camera movement by world bounds
    if (simulation && renderer) {
        Rectangle viewport = renderer->getViewport();
        Rectangle worldRectangle = simulation->getWorld()->getWorldRectangle();
        Vector2 max(
                worldRectangle.w - viewport.w,
                worldRectangle.h - viewport.h
        );
        if (camera.x < worldRectangle.x) camera.x = worldRectangle.x;
        if (camera.y < worldRectangle.y) camera.y = worldRectangle.y;
        if (max.x < camera.x) camera.x = max.x;
        if (max.y < camera.y) camera.y = max.y;
    }

    //Never let to be less than 0
    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
}

void GUIGameRoot::getCameraRectangle(Rectangle& rectangle) {
    //Setup the viewport by setting the corner from camera
    rectangle.set(renderer->getViewport());
    rectangle.x += camera.x;
    rectangle.y += camera.y;
}

Player* GUIGameRoot::getUserPlayer() {
    return userPlayer;
}

void GUIGameRoot::setUserPlayer(Player* player) {
    userPlayer = player;
}

bool GUIGameRoot::eventWindowChanged(Window* window) {
    //Refresh camera since window has changed
    setCamera(camera);
    return GUIRoot::eventWindowChanged(window);
}
