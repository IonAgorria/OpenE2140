//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_ASSET_WORLD_LEVEL_H
#define OPENE2140_ASSET_WORLD_LEVEL_H

#include "engine/assets/asset_world.h"

#define LEVEL_SIZE_MAX 128
#define MONEY_PER_TILE 4000

struct TilePrototype;

/**
 * World information asset
 */
class AssetWorldLevel : public AssetWorld {
private:
    /**
     * Stores the dimensions of this world
     * Since level data sections are length fixed its convenient to know what size to expect being useful
     */
    Vector2 levelSize;

public:
    /**
     * Constructor
     */
    AssetWorldLevel(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize);

    std::string toString() const override;

    void dimensions(Vector2& size) override;

    std::string name() override;

    asset_path_t tileset() override;

    void tiles(std::vector<TilePrototype>& tiles) override;

    void players(std::vector<PlayerPrototype>& players) override;

    void entities(std::vector<EntityPrototype>& entities) override;
};

#endif //OPENE2140_ASSET_WORLD_LEVEL_H
