//
// Created by Ion Agorria on 30/05/19
//

#include "asset_world_level.h"

AssetWorldLevel::AssetWorldLevel(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize)
                                : AssetWorld(path, file, fileOffset, fileSize) {
    //Read basic data
    seek(0xF627, true);
    unsigned int w, h;
    if (!readAll(w) || !readAll(h))  {
        error = "Error reading size\n" + error;
        return;
    }
    levelSize.set(w, h);
}

std::string AssetWorldLevel::toString() const {
    return "AssetWorldLevel(" + toStringContent() + ")";
}

void AssetWorldLevel::dimensions(Vector2& size) {
    size.set(levelSize);
}

std::string AssetWorldLevel::name() {
    seek(0, true);
    std::string name;
    unsigned char c;
    for (int i = 0; i < 32; ++i) {
        if (!readAll(c)) {
            error = "Error reading name\n" + error;
            break;
        }
        //Null termination
        if (c == 0) break;
        //Some versions have non printable characters instead of space
        if (c >= 0x7f) c = ' ';
        //Ignore spaces if nothing was added
        if (name.empty() && c == ' ') continue;
        name += c;
    }
    return name;
}

asset_path_t AssetWorldLevel::tileset() {
    seek(0xF653, true);
    unsigned int id;
    if (!readAll(id)) {
        error = "Error reading tileset\n" + error;
        return nullptr;
    }
    return "MIX/SPRT" + std::to_string(id);
}

void AssetWorldLevel::tiles(std::vector<TilePrototype>& tiles) {
    for (int y = 0; y < levelSize.y; ++y) {
        for (int x = 0; x < levelSize.x; ++x) {
            int i = (x + levelSize.x * y);
            //Get tile index
            byte_t tile_index;
            seek(0x8020 + i, true);
            if (!readAll(tile_index)) {
                error = "Error reading tile terrain\n" + error;
                return;
            }

            //Get tile flags
            unsigned short tile_flags;
            seek(0x0020 + (i * 2), true);
            if (!readAll(tile_flags)) {
                error = "Error reading tile flags\n" + error;
                return;
            }

            //Setup tile
            TilePrototype tile;
            tile.index = tile_index;
            switch (tile_flags) {
                case 0x0100: //Free
                    tile.isPassable = true;
                    break;
                case 0x0200: //Water
                    tile.isWater = true;
                    tile.isImmutable = true;
                    break;
                case 0x0800: //Shore
                    tile.isPassable = true;
                    tile.isShore = true;
                    tile.isImmutable = true;
                    break;
                case 0x1100: //Blocked
                    tile.isImmutable = true;
                    break;
                case 0x2100: //Ore
                    tile.isPassable = true;
                    tile.ore = MONEY_PER_TILE;
                    break;
                case 0x4100: //Sand
                    tile.isPassable = true;
                    tile.isSand = true;
                    break;
            }
            tiles.emplace_back(tile);
        }
    }
}

void AssetWorldLevel::entities(std::vector<EntityPrototype>& entities) {
    seek(0xC02B, true);
}

void AssetWorldLevel::players(std::vector<PlayerPrototype>& players) {
    seek(0xF657, true);
    for (int i = 0; i < 6; ++i) {
        byte_t index;
        if (!readAll(index)) {
            error = "Error reading tiles terrain\n" + error;
            return;
        }
        PlayerPrototype player;
        player.id = index;
        players.emplace_back(player);
    }
}