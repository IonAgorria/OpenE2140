//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_ASSET_WORLD_LEVEL_H
#define OPENE2140_ASSET_WORLD_LEVEL_H

#include "engine/assets/asset_world.h"

/**
 * World information asset
 */
class AssetWorldLevel : public AssetWorld {
private:
public:
    AssetWorldLevel(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize);

    /**
     * @return string version of this asset
     */
    std::string toString() const override;

    /**
     * @return name of this world
     */
    std::string name() override;

    /**
     * @return asset to use as tileset
     */
    asset_path_t tileset() override;

    /**
     * @return players data
     */
    void players(std::vector<PlayerPrototype>& entities) override;

    /**
     * @return entities data
     */
    void entities(std::vector<EntityPrototype>& entities) override;
};

#endif //OPENE2140_ASSET_WORLD_LEVEL_H
