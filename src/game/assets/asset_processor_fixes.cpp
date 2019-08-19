//
// Created by Ion Agorria on 21/04/19
//

#include "engine/assets/asset_manager.h"
#include "engine/graphics/palette.h"
#include "engine/graphics/image.h"
#include "game/core/constants.h"
#include "asset_processor_fixes.h"

void AssetProcessorFixes::refreshAssets() {
    //Fix the radar sprites shadow by removing it since it looks weird when doing full turn
    std::shared_ptr<Image> radar = manager->getImage("MIX/SPRU0/587");
    if (radar) {
        std::shared_ptr<Palette> radarPalette = radar->getPalette();
        std::shared_ptr<Palette> palette = std::make_shared<Palette>(radarPalette->length(), radarPalette->isExtra());
        palette->setColors(radarPalette.get());
        palette->setColor(PALETTE_SHADOW, Color::CLEAR);
        palette->updateTexture();
        for (int i = 587; i <= 596; ++i) {
            std::shared_ptr<Image> image = manager->getImage("MIX/SPRU0/" + std::to_string(i));
            image->setPalette(palette);
        }
    }
}