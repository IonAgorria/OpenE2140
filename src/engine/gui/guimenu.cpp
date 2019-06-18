//
// Created by Ion Agorria on 11/05/19
//

#include "core/engine.h"
#include "guimenu.h"

GUIMenu::GUIMenu(std::shared_ptr<Engine> engine): engine(engine) {
}

GUIMenu::~GUIMenu() {
}

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
