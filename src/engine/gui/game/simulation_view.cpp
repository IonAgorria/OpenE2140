//
// Created by Ion Agorria on 30/8/19
//
#include "engine/graphics/renderer.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/world/world.h"
#include "gui_game_root.h"
#include "simulation_view.h"

void SimulationView::rootChanged() {
    GUIView::rootChanged();
    gameRoot = dynamic_cast<GUIGameRoot*>(root);
    if (root && !gameRoot) {
        BUG("View root is not game root");
    }
}

void SimulationView::draw() {
    //Remove visible entities list
    gameRoot->visibleEntities.clear();

    //Set renderer camera for this view and subviews
    const Vector2& camera = gameRoot->getCamera();
    renderer->changeCamera(camera.x, camera.y);

    //Draw the simulation content if any
    Simulation* simulation = gameRoot->getSimulation();
    if (simulation) {
        //Get the camera rectangle
        Rectangle cameraRectangle;
        gameRoot->getCameraRectangle(cameraRectangle);

        //Obtain a slightly bigger rectangle
        //This avoids seeing empty parts of world when scrolling
        Rectangle extraRectangle(cameraRectangle);
        extraRectangle.grow(simulation->tileSize);

        //Draw simulation
        simulation->draw(extraRectangle, gameRoot->visibleEntities);
    }

    //Draw subviews
    GUIView::draw();

    //Restore camera for UI drawing
    renderer->changeCamera(0, 0);
}

