//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_ASSETPALETTE_H
#define OPENE2140_ASSETPALETTE_H

#include <src/engine/simulation/enviroment/world_prototypes.h>
#include "asset.h"

/**
 * World information asset
 */
class AssetWorld : public Asset {
private:
public:
    /**
     * Constructor for asset
     *
     * @param path where this asset is located
     * @param file to use when accessing data
     * @param fileOffset where asset data start in file
     * @param fileSize of asset data in file, 0 for unknown/until end
     */
    AssetWorld(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize);

    /**
     * Asset destructor
     */
    virtual ~AssetWorld() = default;

    /**
     * @return string version of this asset
     */
    std::string toString() const override;

    /**
     * @return name of this world
     */
    virtual std::string name() const = 0;

    /**
     * @return asset to use as tileset
     */
    virtual asset_path_t tileset() const = 0;

    /**
     * @return players data
     */
    virtual void players(std::vector<PlayerPrototype>& entities) const = 0;

    /**
     * @return entities data
     */
    virtual void entities(std::vector<EntityPrototype>& entities) const = 0;
};

#endif //OPENE2140_ASSETPALETTE_H
