//
// Created by Ion Agorria on 20/05/18
//
#include "engine/graphics/renderer.h"
#include "engine/assets/asset_level.h"
#include "engine/simulation/simulation.h"
#include "world.h"

World::World(AssetLevel* assetLevel, std::unordered_map<unsigned int, Image*>& tilesetImages) :
    tilesetImages(std::move(tilesetImages)),
    tilesetIndex(assetLevel->tilesetIndex()),
    tileSize(static_cast<int>(assetLevel->tileSize())),
    tileSizeHalf(tileSize / 2)
    {
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
    if (Utils::isFlag(FLAG_DEBUG)) {
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
        Vector2 pos(
                static_cast<int>(i % realRectangle.w),
                static_cast<int>(i / realRectangle.h)
        );
        std::unique_ptr<Tile> tile = std::make_unique<Tile>(i, pos);
        tile->setPrototype(prototype);
        tiles.emplace_back(std::move(tile));
    }

    //Adjust tile images array to tiles size
    tilesImages.resize(tiles.size());

    //Generate adjacent tiles now that all tiles are present
    for (std::unique_ptr<Tile>& tile : tiles) {
        Vector2 position = tile->position;
        int mx = position.x;
        int my = position.y;
        for (int y = -1; y <= 1; y++) {
            for (int x = -1; x <= 1; x++) {
                if (x == 0 && y == 0) continue;
                if (!realRectangle.isInside(mx + x, my + y)) continue;
                Tile* other = getTile(mx + x, my + y);
                if (!other) continue;
                tile->adjacents.emplace_back(other);
            }
        }
    }
}

World::~World() {
    realRectangle.set(0);
    tileRectangle.set(0);
    worldRectangle.set(0);
    tiles.clear();
    tilesImages.clear();
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
            Image* image = tilesImages.at(index);
            if (!image) {
                continue;
            }
            renderer->draw(
                static_cast<float>(tileSizeHalf + (x * drawTileSize)),
                static_cast<float>(tileSizeHalf + (y * drawTileSize)),
                static_cast<float>(drawTileSize),
                static_cast<float>(drawTileSize),
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

std::vector<std::unique_ptr<Tile>>& World::getTiles() {
    return tiles;
}

Tile* World::getTile(tile_index_t index) {
    if (index < 0 || index >= tiles.size()) {
        return nullptr;
    }
    return tiles.at(index).get();
}

Tile* World::getTile(unsigned int x, unsigned int y) {
    return getTile(x + realRectangle.w * y);
}

Tile* World::getTile(const Vector2& position) {
    return getTile(position.x / tileSize, position.y / tileSize);
}

void World::toTileVector(const Vector2& vector, Vector2& result) {
    result.set(vector.x / tileSize, vector.y / tileSize);
}

void World::toWorldVector(const Vector2& vector, Vector2& result) {
    result.set(vector.x * (tileSize + tileSizeHalf), vector.y * (tileSize + tileSizeHalf));
}

void World::toTileRectangle(const Rectangle& rectangle, Rectangle& result) {
    result.set(
            rectangle.x / tileSize, rectangle.y / tileSize,
            rectangle.w / tileSize, rectangle.h / tileSize
    );
}

void World::toWorldRectangle(const Rectangle& rectangle, Rectangle& result) {
    result.set(
            rectangle.x * (tileSize + tileSizeHalf), rectangle.y * (tileSize + tileSizeHalf),
            rectangle.w * (tileSize + tileSizeHalf), rectangle.h * (tileSize + tileSizeHalf)
    );
}

Image* World::calculateTileImage(Tile& tile) {
    tile.isImageDirty = false;
    Image* image = tilesetImages[tile.tilesetIndex];
    //TODO check if tile has damage such as fire/weapon and select the image
    return image;
}
