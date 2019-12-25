//
// Created by Ion Agorria on 20/05/18
//
#include "engine/graphics/renderer.h"
#include "engine/assets/asset_level.h"
#include "engine/simulation/simulation.h"
#include "world.h"

World::World(AssetLevel* assetLevel, std::unordered_map<unsigned int, Image*>& tilesetImages) :
    tilesetImages(std::move(tilesetImages)),
    tileSize(static_cast<int>(assetLevel->tileSize())),
    tilesetIndex(assetLevel->tilesetIndex())
    {
    log = Log::get("World");
    debugTiles = Utils::isFlag(FLAG_DEBUG_ALL);

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

void World::draw(Renderer* renderer, const Rectangle& rectangle, int scaling) {
    //Do pixel to tile conversions
    int viewX = rectangle.x;
    int viewY = rectangle.y;
    int tileStartX = std::max(tileRectangle.x, viewX / tileSize);
    int tileStartY = std::max(tileRectangle.y, viewY / tileSize);
    int tileEndX = std::min(tileRectangle.w, (viewX + rectangle.w) / tileSize);
    int tileEndY = std::min(tileRectangle.h, (viewY + rectangle.h) / tileSize);
    int drawTileSize = tileSize * scaling;
    //Iterate each tile inside rectangle
    std::vector<Rectangle> rectangles;
    for (int y = tileStartY; y < tileEndY; ++y) {
        for (int x = tileStartX; x < tileEndX; ++x) {
            //Get current image of tile and draw it
            int index = x + realRectangle.w * y;
            Image* image = tilesImages.at(index);
            if (!image) {
                continue;
            }
            int tx = x * drawTileSize;
            int ty = y * drawTileSize;
            renderer->drawImage(
                    static_cast<float>(tx),
                    static_cast<float>(ty),
                    static_cast<float>(drawTileSize),
                    static_cast<float>(drawTileSize),
                    *image,
                    nullptr
            );

            //Store rectangles for later
            if (debugTiles) {
                rectangles.emplace_back(tx, ty, drawTileSize, drawTileSize);
            }
        }
    }

    //Draw debug rectangles
    if (debugTiles) {
        for (const Rectangle& rect : rectangles) {
            renderer->drawRectangle(rect, 1, Color::DEBUG_WORLD);
        }
    }
}

int World::getTileSize() {
    return tileSize;
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

Tile* World::getTile(tile_index_t index) const {
    if (index < 0 || index >= tiles.size()) {
        return nullptr;
    }
    return tiles.at(index).get();
}

Tile* World::getTile(unsigned int x, unsigned int y) const {
    return getTile(x + realRectangle.w * y);
}

Tile* World::getTile(const Vector2& position) const {
    return getTile(position.x / tileSize, position.y / tileSize);
}

Image* World::calculateTileImage(Tile& tile) {
    tile.isImageDirty = false;
    Image* image = tilesetImages[tile.tilesetIndex];
    //TODO check if tile has damage such as fire/weapon and select the image
    return image;
}
