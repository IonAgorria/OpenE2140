//
// Created by Ion Agorria on 29/04/18
//
#ifndef OPENE2140_ASSET_LEVEL_GAME_H
#define OPENE2140_ASSET_LEVEL_GAME_H

#include "game/core/constants.h"
#include "engine/assets/asset_level.h"

#define TILESET_MAX 252
#define LEVEL_SIZE_MAX 128
#define PLAYERS_MAX 6
#define ENTITIES_PER_SECTION 256
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

    /**
     * Tileset index used by this level
     */
    unsigned int levelTilesetIndex;

public:
    /**
     * Constructor
     */
    AssetLevelGame(const asset_path_t& path, std::shared_ptr<File> file, long fileOffset, long fileSize);

    TYPE_NAME_OVERRIDE(AssetLevelGame)

    /**
     * Does game level to engine player id conversion
     *
     * @param player to convert
     * @return id
     */
    player_id_t getPlayerId(byte_t player);

    void dimensions(Vector2& size) override;

    std::string name() override;

    unsigned int tileSize() override;

    asset_path_t tilePath(size_t index) override;

    size_t tilesetSize() override;

    unsigned int tilesetIndex() override;

    void tiles(std::vector<TilePrototype>& tiles) override;

    void players(std::vector<PlayerPrototype>& players) override;

    void entities(std::vector<EntityPrototype>& entities) override;
};

#endif //OPENE2140_ASSET_LEVEL_GAME_H
