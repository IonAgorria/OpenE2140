//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_TILE_H
#define OPENE2140_TILE_H

#include "engine/core/common.h"
#include "world_prototypes.h"

class Entity;

/**
 * Stores each tile information
 */
class Tile: public TilePrototype {
public:
    /**
     * Tile index in the world
     */
    const tile_index_t index;

    /**
     * Tile position in the world
     */
    const Vector2 position;

    /**
     * Flag for tile image being dirty
     */
    bool isImageDirty = true;

    /**
     * Adjacent tiles to this tile
     */
    std::vector<Tile*> adjacents;

    /**
     * Entities inside this tile
     */
    std::vector<std::shared_ptr<Entity>> entities;

    /**
     * Type of damage on this tile (weapons, fire, reactor crate...)
     */
     //TODO

    /**
     * Constructor
     */
    Tile(tile_index_t index, Vector2& position);

    /**
     * Destructor
     */
    virtual ~Tile() = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Tile)

    /**
     * Set data from prototype
     */
    void setPrototype(TilePrototype prototype);
};

#endif //OPENE2140_TILE_H
