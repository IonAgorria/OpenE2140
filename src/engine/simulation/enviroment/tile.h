//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_TILE_H
#define OPENE2140_TILE_H

#include "core/common.h"

class Entity;
/**
 * Stores each tile information
 */
class Tile {
public:
    /**
     * Flag for tile image being dirty
     */
    bool isImageDirty = true;

    /**
     * Flag for water
     */
    bool isWater = false;

    /**
     * Flag for passable
     */
    bool isPassable = false;

    /**
     * Flag for immutable (is not affected by explosions or other stuff
     */
    bool isImmutable = false;

    /**
     * Entities inside this tile
     */
     std::vector<Entity*> entities;

    /**
     * Type of damage on this tile (weapons, fire, reactor crate...)
     */
     //TODO

    /**
     * Tile constructor
     */
    Tile();

    /**
     * Tile destructor
     */
    ~Tile() = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Tile)

    /**
     * Set tile as reactor crate
     */
     void setReactorCrate();
};

#endif //OPENE2140_TILE_H
