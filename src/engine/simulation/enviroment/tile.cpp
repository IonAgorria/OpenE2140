//
// Created by Ion Agorria on 20/05/18
//
#include "tile.h"

Tile::Tile() {
}

void Tile::setPrototype(TilePrototype prototype) {
    this->index = prototype.index;
    this->isWater = prototype.isWater;
    this->isShore = prototype.isShore;
    this->isPassable = prototype.isPassable;
    this->isImmutable = prototype.isImmutable;
    this->ore = prototype.ore;
    this->isSand = prototype.isSand;
}

void Tile::setReactorCrate() {
    isPassable = false;
    isImmutable = true;
    isImageDirty = true;
    //TODO set damage type
}
