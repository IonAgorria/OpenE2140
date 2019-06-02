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
    worldRectangle.set(Vector2(), size);
    log->debug("Size: '" + size.toString() + "'");

    //Load tiles
    std::vector<TilePrototype> tilePrototypes;
    assetLevel->tiles(tilePrototypes);
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
    int drawStartX = rectangle.x % tileSize;
    int drawStartY = rectangle.y % tileSize;
    int tileStartX = std::max(worldRectangle.x, rectangle.x / tileSize);
    int tileStartY = std::max(worldRectangle.y, rectangle.y / tileSize);
    int tileEndX = std::min(worldRectangle.w, (rectangle.x + rectangle.w) / tileSize + 1);
    int tileEndY = std::min(worldRectangle.h, (rectangle.y + rectangle.h) / tileSize + 1);

    //Create rectangle for drawing operations
    int drawTileSize = tileSize * scaling;
    //Iterate each tile inside rectangle
    for (int y = tileStartY; y < tileEndY; y++) {
        for (int x = tileStartX; x < tileEndX; x++) {
            //Get current image of tile and draw it
            int index = x + worldRectangle.w * y;
            std::shared_ptr<Image> image = tilesImages.at(index);
            if (!image) {
                continue;
            }
            renderer->draw(
                drawStartX + (x * drawTileSize),
                drawStartY + (y * drawTileSize),
                drawTileSize,
                drawTileSize,
                0,
                *image,
                nullptr
            );
        }
    }
}


Tile* World::getTile(unsigned long index) {
    if (index < 0 || index >= tiles.size()) {
        return nullptr;
    }
    return tiles.at(index).get();
}

Tile* World::getTile(unsigned long x, unsigned long y) {
    return getTile(x + worldRectangle.w * y);
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
