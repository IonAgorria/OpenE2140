//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_WORLD_H
#define OPENE2140_WORLD_H

#include "engine/core/macros.h"
#include "engine/core/error_possible.h"
#include "engine/io/log.h"
#include "engine/math/rectangle.h"
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
     * Real size of world in tiles
     */
    Rectangle realRectangle;

    /**
     * Rectangle containing this world in tiles
     */
    Rectangle tileRectangle;

    /**
     * Rectangle containing this world
     */
    Rectangle worldRectangle;

    /**
     * Images in tileset
     */
    std::unordered_map<unsigned int, Image*> tilesetImages;

    /**
     * Images to be drawn for each tile
     */
    std::vector<Image*> tilesImages;

    /**
     * Tiles information
     */
    std::vector<std::unique_ptr<Tile>> tiles;

public:
    /**
     * Flag for enabling debugging tiles
     */
    bool debugTiles = false;

    /**
     * Tileset index used in this world
     */
    const unsigned int tilesetIndex;

    /**
     * Tile image size
     */
    const int tileSize;

    /**
     * Tile image size half
     */
    const int tileSizeHalf;

    /**
     * World scaling
     */
    const int scaling = 1;

    /**
     * World constructor
     */
    World(AssetLevel* assetLevel, std::unordered_map<unsigned int, Image*>& tilesetImages);

    /**
     * World destructor
     */
    ~World() override;

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
     * @return real sized rectangle of this world in tiles
     */
    const Rectangle& getRealRectangle();

    /**
     * @return cropped rectangle containing this world in tiles
     */
    const Rectangle& getTileRectangle();

    /**
     * @return cropped rectangle containing this world
     */
    const Rectangle& getWorldRectangle();

    /**
     * @return tiles in this world
     */
    std::vector<std::unique_ptr<Tile>>& getTiles();

    /**
     * Tile in specified tile index
     *
     * @param index of tile in world
     * @return tile if valid
     */
    Tile* getTile(tile_index_t index);

    /**
     * Tile in specified tile position
     *
     * @param index of tile in world
     * @return tile if valid
     */
    Tile* getTile(unsigned int x, unsigned int y);

    /**
     * Tile in specified world position
     *
     * @param index of tile in world
     * @return tile if valid
     */
    Tile* getTile(const Vector2& position);

    /**
     * Translates world vector to tile vector
     *
     * @param vector to convert
     * @param result to write result
     * @return tile if valid
     */
    void toTileVector(const Vector2& vector, Vector2& result);

    /**
     * Translates tile vector to world vector
     *
     * @param vector to convert
     * @param result to write result
     * @return tile if valid
     */
    void toWorldVector(const Vector2& vector, Vector2& result);

    /**
     * Translates world rectangle to tile rectangle
     *
     * @param rectangle to convert
     * @param result to write result
     * @return tile if valid
     */
    void toTileRectangle(const Rectangle& rectangle, Rectangle& result);

    /**
     * Translates tile rectangle to world rectangle
     *
     * @param rectangle to convert
     * @param result to write result
     * @return tile if valid
     */
    void toWorldRectangle(const Rectangle& rectangle, Rectangle& result);

    /**
     * Calculates the image for the tile
     * @param tile
     * @return
     */
    Image* calculateTileImage(Tile& tile);
};


#endif //OPENE2140_WORLD_H
