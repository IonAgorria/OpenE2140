//
// Created by Ion Agorria on 11/05/19
//

#include "engine/core/engine.h"
#include "gui_menu.h"

bool GUIMenu::eventMouseClick(Window* window, int x, int y, int button, bool press) {
    return GUIView::mouseClick(x, y, button, press);
}

bool GUIMenu::eventMouseWheel(Window* window, int x, int y) {
    return GUIView::mouseWheel(x, y);
}

bool GUIMenu::eventMouseMove(Window* window, int x, int y) {
    return GUIView::mouseMove(x, y);
}

bool GUIMenu::eventKeyChange(Window* window, input_key_t& key) {
    return GUIView::keyChange(key);
}

void GUIMenu::update() {
    eventUpdate();
    GUIView::update();
}
