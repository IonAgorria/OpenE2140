//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_TILE_H
#define OPENE2140_TILE_H

#include <core/config.h>

/**
 * Stores each tile information
 */
class Tile {
public:
    /**
     * Flag for water
     */
    bool isWater;

    /**
     * Flag for passable
     */
    bool isPassable;

    /**
     * Flag for inmutability (is not affected by explosions or other stuff
     */
    bool isInmutable;

    /**
     * Entities inside this tile
     */
     //TODO std::list<Entity> entities;

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
