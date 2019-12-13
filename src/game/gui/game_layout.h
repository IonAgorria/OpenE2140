//
// Created by Ion Agorria on 1/9/19
//
#ifndef OPENE2140_GAME_LAYOUT_H
#define OPENE2140_GAME_LAYOUT_H

#include "engine/gui/game/gui_game_root.h"
#include "engine/gui/game/selection_overlay.h"
#include "engine/gui/game/simulation_view.h"
#include "engine/gui/game/camera_view.h"

/**
 * Provides the running game layout of views
 */
class GameLayout: public GUIGameRoot {
public:
    /**
     * Type name
     */
    TYPE_NAME_OVERRIDE(GameLayout)

    void rootActive(std::shared_ptr<Engine> engine) override {
        GUIGameRoot::rootActive(engine);
        if (engine) {
            //Add the simulation view
            auto simulationView = std::make_unique<SimulationView>();
            simulationView->addView(std::make_unique<SelectionOverlay>());
            simulationView->addView(std::make_unique<CameraView>());
            addView(std::move(simulationView));
        }

    }

    void layout() override {
        for (auto& view : views) {
            view->setRectangle(rectangle);
        }
        GUIGameRoot::layout();
    }
};

#endif //OPENE2140_GAME_LAYOUT_H
