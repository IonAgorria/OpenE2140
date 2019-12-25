//
// Created by Ion Agorria on 20/05/18
//
#ifndef OPENE2140_TILE_H
#define OPENE2140_TILE_H

#include "engine/core/macros.h"
#include "world_prototypes.h"

class Entity;

/**
 * Stores each tile information
 */
class Tile: public TilePrototype, public virtual IToString {
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
    TYPE_NAME_OVERRIDE(Tile);

    /**
     * Set data from prototype
     */
    void setPrototype(TilePrototype prototype);

    /**
     * Adds a entity to this tile and tile to entity tiles list
     *
     * @param entity
     * @return
     */
    bool addEntity(const std::shared_ptr<Entity>& entity, bool clearTiles = true);

    /**
     * Removes an entity from this tile and tile from entity tiles list
     *
     * @param id of entity to remove
     * @return
     */
    bool removeEntity(entity_id_t id);

    /*
     * IToString
     */

    std::string toStringContent() const override;
};

#endif //OPENE2140_TILE_H
