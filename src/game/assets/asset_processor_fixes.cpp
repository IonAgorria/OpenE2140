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
    std::shared_ptr<Image> baseImage = manager->getImage("MIX/SPRU0/587");
    if (baseImage) {
        std::shared_ptr<Palette> basePalette = baseImage->getPalette();
        std::shared_ptr<Palette> palette = std::make_shared<Palette>(basePalette->length(), basePalette->isExtra());
        palette->setColors(basePalette.get());
        palette->setColor(PALETTE_SHADOW, Color::CLEAR);
        palette->updateTexture();
        for (int i = 587; i <= 596; ++i) {
            std::shared_ptr<Image> image = manager->getImage("MIX/SPRU0/" + std::to_string(i));
            image->setPalette(palette);
        }
    }
    //Add shadow to objects
    for (std::string variant : ENTITY_OBJECTS_VARIANTS) {
        //Walls
        //TODO
        //Trees
        baseImage = manager->getImage("MIX/SPRO" + variant + "/93");
        if (baseImage) {
            std::shared_ptr<Palette> basePalette = baseImage->getPalette();
            std::shared_ptr<Palette> palette = std::make_shared<Palette>(basePalette->length(), basePalette->isExtra());
            palette->setColors(basePalette.get());
            palette->setColor(PALETTE_SHADOW, Color::SHADOW_MAIN);
            palette->updateTexture();
            for (int i = 93; i <= 104; ++i)  {
                std::shared_ptr<Image> image = manager->getImage("MIX/SPRO" + variant + "/" + std::to_string(i));
                if (!image) continue;
                image->setPalette(palette);
            }
        }
    }
    //Fix the conveyor belt black spot
    baseImage = manager->getImage("MIX/SPRB0/9");
    if (baseImage) {
        std::shared_ptr<Palette> basePalette = baseImage->getPalette();
        std::shared_ptr<Palette> palette = std::make_shared<Palette>(basePalette->length(), basePalette->isExtra());
        palette->setColors(basePalette.get());
        palette->setColor(PALETTE_MAX_INDEX, Color::CLEAR);
        palette->updateTexture();
        for (int i = 9; i <= 11; ++i) {
            std::shared_ptr<Image> image = manager->getImage("MIX/SPRB0/" + std::to_string(i));
            image->setPalette(palette);
        }
    }
}