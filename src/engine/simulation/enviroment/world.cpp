//
// Created by Ion Agorria on 20/05/18
//
#include "engine/graphics/renderer.h"
#include "assets/asset_level.h"
#include "engine/simulation/simulation.h"
#include "world.h"

World::World(AssetLevel* assetLevel, std::unordered_map<unsigned int, std::shared_ptr<Image>>& tilesetImages)
            : tilesetImages(std::move(tilesetImages)) {
    log = Log::get("World");

    //Set dimensions
    Vector2 size;
    assetLevel->dimensions(size);
    error = assetLevel->getError();
    if (hasError()) {
        error = "Error at getting level dimensions\n" + error;
        log->error(error);
        return;
    }
    realRectangle.set(Vector2(0), size);
    if (Utils::isDebug()) {
        tileRectangle.set(realRectangle);
    } else {
        tileRectangle.set(
                std::max(realRectangle.x, 1),
                std::max(realRectangle.y, 1),
                std::max(0, realRectangle.w - 1),
                std::max(0, realRectangle.h - 1)
        );
    }
    worldRectangle.set(tileRectangle * Rectangle(tileSize));
    log->debug("Size: '" + size.toString() + "'");

    //Load tiles
    std::vector<TilePrototype> tilePrototypes;
    assetLevel->tiles(tilePrototypes);
    error = assetLevel->getError();
    if (hasError()) {
        error = "Error at getting level tiles\n" + error;
        log->error(error);
        return;
    }
    for (size_t i = 0; i < tilePrototypes.size(); ++i) {
        TilePrototype prototype = tilePrototypes.at(i);
        std::unique_ptr<Tile> tile = std::make_unique<Tile>();
        tile->setPrototype(prototype);
        tiles.emplace_back(std::move(tile));
    }
    tilesImages.resize(tiles.size());
}

World::~World() {
}

void World::update() {
    size_t size = tiles.size();
    for (size_t i = 0; i < size; ++i) {
        Tile& tile = *tiles[i];

        //Update image for tile
        if (tile.isImageDirty) {
            tilesImages[i] = calculateTileImage(tile);
        }
    }
}

void World::draw(Renderer* renderer, const Rectangle& rectangle) {
    //Do pixel to tile conversions
    int viewX = rectangle.x + tileSizeHalf;
    int viewY = rectangle.y + tileSizeHalf;
    int tileStartX = std::max(tileRectangle.x, viewX / tileSize - 1);
    int tileStartY = std::max(tileRectangle.y, viewY / tileSize - 1);
    int tileEndX = std::min(tileRectangle.w, (viewX + rectangle.w) / tileSize + 1);
    int tileEndY = std::min(tileRectangle.h, (viewY + rectangle.h) / tileSize + 1);
    int drawTileSize = tileSize * scaling;
    //Iterate each tile inside rectangle
    for (int y = tileStartY; y < tileEndY; ++y) {
        for (int x = tileStartX; x < tileEndX; ++x) {
            //Get current image of tile and draw it
            int index = x + realRectangle.w * y;
            std::shared_ptr<Image> image = tilesImages.at(index);
            if (!image) {
                continue;
            }
            renderer->draw(
                tileSizeHalf + (x * drawTileSize),
                tileSizeHalf + (y * drawTileSize),
                drawTileSize,
                drawTileSize,
                0,
                *image,
                nullptr
            );
        }
    }
}

const Rectangle& World::getRealRectangle() {
    return realRectangle;
}

const Rectangle& World::getTileRectangle() {
    return tileRectangle;
}

const Rectangle& World::getWorldRectangle() {
    return worldRectangle;
}

Tile* World::getTile(unsigned long index) {
    if (index < 0 || index >= tiles.size()) {
        return nullptr;
    }
    return tiles.at(index).get();
}

Tile* World::getTile(unsigned long x, unsigned long y) {
    return getTile(x + realRectangle.w * y);
}

Tile* World::getTile(const Vector2& position) {
    return getTile(position.x / tileSize, position.y / tileSize);
}

void World::toTilePosition(const Vector2& position, Vector2& result) {
    result.set(position.x / tileSize, position.y / tileSize);
}

void World::toWorldPosition(const Vector2& position, Vector2& result) {
    result.set(position.x * (tileSize + tileSizeHalf), position.y * (tileSize + tileSizeHalf));
}

std::shared_ptr<Image> World::calculateTileImage(Tile& tile) {
    tile.isImageDirty = false;
    std::shared_ptr<Image> image = tilesetImages[tile.index];
    //TODO check if tile has damage such as fire/weapon and select the image
    return image;
}
