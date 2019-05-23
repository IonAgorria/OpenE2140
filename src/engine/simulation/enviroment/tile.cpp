//
// Created by Ion Agorria on 20/05/18
//
#include "tile.h"

Tile::Tile() {
}

void Tile::setReactorCrate() {
    isPassable = false;
    isImmutable = true;
    isImageDirty = true;
    //TODO set damage type
}
