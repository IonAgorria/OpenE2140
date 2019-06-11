//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_TILE_H
#define OPENE2140_TILE_H

#include "core/common.h"
#include "world_prototypes.h"

class Entity;

/**
 * Stores each tile information
 */
class Tile: public TilePrototype {
public:
    /**
     * Flag for tile image being dirty
     */
    bool isImageDirty = true;

    /**
     * Entities inside this tile
     */
     std::vector<Entity*> entities;

    /**
     * Type of damage on this tile (weapons, fire, reactor crate...)
     */
     //TODO

    /**
     * Constructor
     */
    Tile();

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

    /**
     * Set tile as reactor crate
     */
     void setReactorCrate();
};

#endif //OPENE2140_TILE_H
