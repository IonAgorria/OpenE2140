//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_ASSET_WORLD_H
#define OPENE2140_ASSET_WORLD_H

#include "simulation/enviroment/world_prototypes.h"
#include "asset.h"

/**
 * World information asset
 */
class AssetWorld : public Asset {
private:
public:
    /**
     * Constructor
     */
    AssetWorld(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize);

    /**
     * @return string version of this asset
     */
    std::string toString() const override;

    /**
     * @return name of this world
     */
    virtual std::string name();

    /**
     * @return asset to use as tileset or null
     */
    virtual asset_path_t tileset();

    /**
     * Writes players data to provided vector
     */
    virtual void players(std::vector<PlayerPrototype>& entities);

    /**
     * Writes entities data to provided vector
     */
    virtual void entities(std::vector<EntityPrototype>& entities);
};

#endif //OPENE2140_ASSET_WORLD_H
