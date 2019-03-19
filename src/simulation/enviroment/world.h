//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_WORLD_H
#define OPENE2140_WORLD_H

#include <vector>
#include "core/common.h"
#include "graphics/window.h"
#include "assets/asset.h"
#include "tile.h"

class World {
private:
    /**
     * Rectangle containing this world, position is always 0, 0
     */
     const Rectangle worldRectangle;

    /**ŧ
     * Images to be drawn
     */
    const std::vector<std::shared_ptr<Image>> tileImages;

    /**
     * Tiles information
     */
    const std::vector<Tile> tiles;

    /**
     * Tile image size
     */
    const int tileSize = 64;

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
     * Draws the world using provided view
     *
     * @param window to use for drawing
     * @param view window
     * @return true if success
     */
     bool draw(Window& window, Rectangle& rectangle);
};


#endif //OPENE2140_WORLD_H
