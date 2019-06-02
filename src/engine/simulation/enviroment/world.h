//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_WORLD_H
#define OPENE2140_WORLD_H

#include "core/common.h"
#include "math/rectangle.h"
#include "tile.h"

class Renderer;
class Image;
class AssetLevel;
class Simulation;

/**
 * Contains the world data such as tiles
 */
class World: public IErrorPossible {
private:
    /**
     * Log for object
     */
    log_ptr log;

    /**
     * Size of world in tiles
     */
    Vector2 worldSize;

    /**
     * Rectangle containing this world, position is always 0, 0
     */
    Rectangle worldRectangle;

    /**
     * Images in tileset
     */
    std::unordered_map<unsigned int, std::shared_ptr<Image>> tilesetImages;

    /**
     * Images to be drawn for each tile
     */
    std::vector<std::shared_ptr<Image>> tilesImages;

    /**
     * Tiles information
     */
    std::vector<std::unique_ptr<Tile>> tiles;

    /**
     * Tile image size
     */
    const int tileSize = 64; //TODO move this to asset level method and override in game, as its game specific

    /**
     * Tile image size half
     */
    const int tileSizeHalf = tileSize / 2;

    /**
     * World scaling
     */
    const int scaling = 1;

public:
    /**
     * World constructor
     */
    World(AssetLevel* assetLevel, std::unordered_map<unsigned int, std::shared_ptr<Image>>& tilesetImages);

    /**
     * World destructor
     */
    ~World();

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(World)

    /**
     * Updates the world state
     */
    void update();

    /**
     * Draws the world using provided view
     *
     * @param renderer to use for drawing
     * @param rectangle the rectangle to draw
     */
    void draw(Renderer* renderer, const Rectangle& rectangle);

    /**
     * @return size of this world in tiles
     */
    const Vector2& getSize();

    /**
     * @return rectangle containing this world
     */
    const Rectangle& getRectangle();

    /**
     * Tile in specified tile index
     *
     * @param index of tile in world
     * @return tile if valid
     */
    Tile* getTile(unsigned long index);

    /**
     * Tile in specified tile position
     *
     * @param index of tile in world
     * @return tile if valid
     */
    Tile* getTile(unsigned long x, unsigned long y);

    /**
     * Tile in specified world position
     *
     * @param index of tile in world
     * @return tile if valid
     */
    Tile* getTile(const Vector2& position);

    /**
     * Translates world position to tile position
     *
     * @param position to convert
     * @param result to write result
     * @return tile if valid
     */
    void toTilePosition(const Vector2& position, Vector2& result);

    /**
     * Translates tile position to world position
     *
     * @param position to convert
     * @param result to write result
     * @return tile if valid
     */
    void toWorldPosition(const Vector2& position, Vector2& result);

    /**
     * Calculates the image for the tile
     * @param tile
     * @return
     */
    std::shared_ptr<Image> calculateTileImage(Tile& tile);
};


#endif //OPENE2140_WORLD_H
