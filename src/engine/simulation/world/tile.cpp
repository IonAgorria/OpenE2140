//
// Created by Ion Agorria on 20/05/18
//
#include "engine/simulation/entity.h"
#include "tile.h"

Tile::Tile(tile_index_t index, Vector2& position): index(index), position(position) {
}

void Tile::setPrototype(TilePrototype prototype) {
    this->tilesetIndex = prototype.tilesetIndex;
    this->tileFlags = prototype.tileFlags;
    this->ore = prototype.ore;
}

bool Tile::addEntity(const std::shared_ptr<Entity>& entity, bool clearTiles) {
    if (!entity) {
        return false;
    }

    //Check if we have the entity already
    for (auto& e : entities) {
        if (e == entity) {
            return true;
        }
    }

    //Add tile and ourselves to tile
    if (clearTiles) {
        entity->clearTiles();
    }
    entity->getTiles().push_back(this);
    entities.push_back(entity);
    entityFlags |= entity->entityFlagsMask;
    return true;
}

bool Tile::removeEntity(entity_id_t id) {
    std::shared_ptr<Entity> deletedEntity;
    for (auto it = entities.begin(); it != entities.end(); ) {
        std::shared_ptr<Entity> entity = *it;
        if (entity->getID() == id) {
            entities.erase(it);
            deletedEntity = entity;
            break;
        }
    }
    //Remove tile from entity tiles and update flags if entity was deleted
    if (deletedEntity) {
        auto& tiles = deletedEntity->getTiles();
        for (auto it_tiles = tiles.begin(); it_tiles != tiles.end(); ) {
            if (*it_tiles == this) {
                tiles.erase(it_tiles);
                continue;
            }
        }
        updateFlags();
    }
    return false;
}

std::string Tile::toStringContent() const {
    return std::to_string(index) + ", " + position.toString();
}

void Tile::updateFlags() {
    entityFlags = 0;
    for (auto& entity : entities) {
        entityFlags |= entity->entityFlagsMask;
    }
}
