//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_ASSET_LEVEL_GAME_H
#define OPENE2140_ASSET_LEVEL_GAME_H

#include "game/core/constants.h"
#include "engine/assets/asset_level.h"

#define TILESET_MAX 252
#define LEVEL_SIZE_MAX 128
#define MONEY_PER_TILE 30 * MONEY_PER_CONTAINER //Each tile provides 30 containers

/**
 * World information asset
 */
class AssetLevelGame : public AssetLevel {
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
    AssetLevelGame(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize);

    std::string toString() const override;

    void dimensions(Vector2& size) override;

    std::string name() override;

    asset_path_t tileset(size_t index) override;

    size_t tilesetSize() override;

    void tiles(std::vector<TilePrototype>& tiles) override;

    void players(std::vector<PlayerPrototype>& players) override;

    void entities(std::vector<EntityPrototype>& entities) override;
};

#endif //OPENE2140_ASSET_LEVEL_GAME_H
