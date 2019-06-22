//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_ASSET_LEVEL_H
#define OPENE2140_ASSET_LEVEL_H

#include "engine/simulation/world/world_prototypes.h"
#include "engine/math/rectangle.h"
#include "asset.h"

/**
 * World information asset
 */
class AssetLevel : public Asset {
private:
public:
    /**
     * Constructor
     */
    AssetLevel(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize);

    /**
     * @return string version of this asset
     */
    std::string toString() const override;

    /**
     * Dimensions of this world
     *
     * @param size
     */
    virtual void dimensions(Vector2& size);

    /**
     * @return name of this world
     */
    virtual std::string name();

    /**
     * @return asset to use as tileset or null
     */
    virtual asset_path_t tilePath(size_t index);

    /**
     * @return tileset size
     */
    virtual size_t tilesetSize();

    /**
     * @return tileset index
     */
    virtual unsigned int tilesetIndex();

    /**
     * Writes tiles data to provided vector
     */
    virtual void tiles(std::vector<TilePrototype>& tiles);

    /**
     * Writes players data to provided vector
     */
    virtual void players(std::vector<PlayerPrototype>& players);

    /**
     * Writes entities data to provided vector
     */
    virtual void entities(std::vector<EntityPrototype>& entities);
};

#endif //OPENE2140_ASSET_LEVEL_H
