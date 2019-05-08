//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_WORLD_H
#define OPENE2140_WORLD_H

#include <vector>
#include "core/common.h"
#include "math/rectangle.h"
#include "assets/asset.h"
#include "tile.h"

class Renderer;
class Image;
/**
 * Contains the world data such as tiles
 */
class World {
private:
    /**
     * Rectangle containing this world, position is always 0, 0
     */
     const Rectangle worldRectangle;

    /**ŧ
     * Images to be drawn
     */
    std::vector<std::shared_ptr<Image>> tileImages;

    /**
     * Tiles information
     */
    std::vector<std::unique_ptr<Tile>> tiles;

    /**
     * Tile image size
     */
    const int tileSize = 64;

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
    World(const asset_path& path);

    /**
     * World destructor
     */
    ~World() = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(World)

    /**
     * Updates the world state
     *
     * @return true if success
     */
    void update();

    /**
     * Draws the world using provided view
     *
     * @param renderer to use for drawing
     * @param view window
     */
    void draw(Renderer& renderer, const Rectangle& rectangle);

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
