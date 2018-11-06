//
// Created by Ion Agorria on 20/05/18
//
#include "world.h"

World::World(const asset_path& path) {
}

bool World::draw(Window& window, Rectangle& rectangle) {
    //Do pixel to tile conversions
    int drawStartX = rectangle.x % tileSize;
    int drawStartY = rectangle.y % tileSize;
    int tileStartX = rectangle.x / tileSize;
    int tileStartY = rectangle.y / tileSize;
    int tileEndX = (rectangle.x + rectangle.w) / tileSize + 1;
    int tileEndY = (rectangle.y + rectangle.h) / tileSize + 1;
    int tileCountX = tileEndX - tileStartX;
    int tileCountY = tileEndY - tileStartY;

    //Create rectangle for drawing operations
    Rectangle destination = Rectangle(0, 0, tileSize * scaling, tileSize * scaling);

    //Iterate each tile inside rectangle
    for (int y = tileStartY; y < tileEndY; y++) {
        for (int x = tileStartX; x < tileEndX; x++) {
            //Get current image of tile
            std::shared_ptr<Image> image = tileImages[x + worldRectangle.w * y];

            //Set the destination position
            destination.x = drawStartX + ((x - tileCountX) * destination.w);
            destination.y = drawStartY + ((y - tileCountY) * destination.h);
            if (!window.draw(*image, destination)) {
                return false;
            }
        }
    }
    return true;
}
