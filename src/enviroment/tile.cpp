//
// Created by Ion Agorria on 20/05/18
//
#include "tile.h"

Tile::Tile() {
    isWater = false;
    isPassable = true;
    isInmutable = false;
}

void Tile::setReactorCrate() {
    isPassable = false;
    isInmutable = true;
    //TODO set damage type
}
