//
// Created by Ion Agorria on 11/05/19
//

#include "engine/core/engine.h"
#include "engine/graphics/renderer.h"
#include "gui_root.h"

void GUIRoot::rootActive(std::shared_ptr<Engine> newEngine) {
    if (newEngine) {
        //Setup
        engine = std::move(newEngine);
        engine->log->debug("Setting as active {0}", type_name());
        root = this;
        renderer = engine->getRenderer();
        simulation = engine->getSimulation();
    } else {
        engine->log->debug("Removing as active {0}", type_name());
        //First remove views
        removeViews();
        //Unset stuff
        renderer = nullptr;
        simulation = nullptr;
        engine.reset();
        root = nullptr;
    }
}

Engine* GUIRoot::getEngine() {
    return engine.get();
}

Simulation* GUIRoot::getSimulation() {
    return simulation;
}

void GUIRoot::update() {
    eventUpdate();
    GUIView::update();
}

bool GUIRoot::eventWindowChanged(Window* window) {
    setRectangle(renderer->getViewport());
    return false;
}

bool GUIRoot::eventMouseClick(Window* window, int x, int y, int button, bool press) {
    return GUIView::mouseClick(x, y, button, press);
}

bool GUIRoot::eventMouseWheel(Window* window, int x, int y) {
    return GUIView::mouseWheel(x, y);
}

bool GUIRoot::eventMouseMove(Window* window, int x, int y) {
    return GUIView::mouseMove(x, y);
}

bool GUIRoot::eventKeyChange(Window* window, input_key_t& key) {
    return GUIView::keyChange(key);
}
