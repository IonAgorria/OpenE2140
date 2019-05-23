//
// Created by Ion Agorria on 11/05/19
//

#include "core/engine.h"
#include "guimenu.h"

GUIMenu::GUIMenu(std::shared_ptr<Engine> engine): engine(engine) {
}

GUIMenu::~GUIMenu() {
}

bool GUIMenu::mouseClick(Window* window, int x, int y, int button, bool press) {
    return GUIView::mouseClick(x, y, button, press);
}

bool GUIMenu::mouseWheel(Window* window, int x, int y) {
    return GUIView::mouseWheel(x, y);
}

bool GUIMenu::mouseMove(Window* window, int x, int y) {
    return GUIView::mouseMove(x, y);
}

bool GUIMenu::keyChange(Window* window, int code, bool press) {
    return GUIView::keyChange(code, press);
}
