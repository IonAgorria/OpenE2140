//
// Created by Ion Agorria on 30/05/19
//

#include "asset_level_game.h"

/*
      r.BaseStream.Position = position + 49195L;
      this.Units = Helper.ReadObjects<Unit>(r);
      r.BaseStream.Position = position + 55337L;
      this.Buildings = Helper.ReadObjects<Building>(r);
      r.BaseStream.Position = position + 57897L;
      this.Trees = Helper.ReadObjects<Tree>(r);

      r.BaseStream.Position = position + 63063L;
      this.Players = Helper.ReadObjects<Player>(r, 6, true);

      r.BaseStream.Position = position + 77391L;
      this.Triggers = Helper.ReadObjects<Trigger>(r, 10, new Func<BinaryReader, Trigger>(Trigger.FromBinaryReader));

      r.BaseStream.Position = position + 94907L;
      this.UnknownObjects = Helper.ReadObjects<UnknownObject>(r, 32);
      r.BaseStream.Position = position + 94907L;

 */

AssetLevelGame::AssetLevelGame(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize) :
                                AssetLevel(path, file, fileOffset, fileSize) {
    //Read basic data
    seek(0xF627, true);
    unsigned int w, h;
    if (!readAll(w) || !readAll(h))  {
        error = "Error reading size\n" + error;
        return;
    }
    levelSize.set(w, h);
}

std::string AssetLevelGame::toString() const {
    return "AssetLevelGame(" + toStringContent() + ")";
}

void AssetLevelGame::dimensions(Vector2& size) {
    size.set(levelSize);
}

std::string AssetLevelGame::name() {
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

asset_path_t AssetLevelGame::tileset(size_t index) {
    if (index >= TILESET_MAX) {
        return "";
    }
    seek(0xF653, true);
    unsigned int id;
    if (!readAll(id)) {
        error = "Error reading tileset\n" + error;
        return nullptr;
    }
    return "MIX/SPRT" + std::to_string(id) + "/" + std::to_string(index);
}

size_t AssetLevelGame::tilesetSize() {
    return TILESET_MAX;
}

void AssetLevelGame::tiles(std::vector<TilePrototype>& tiles) {
    for (int y = 0; y < levelSize.y; ++y) {
        for (int x = 0; x < levelSize.x; ++x) {
            //This is not a typo, tiles are set this way
            int i = y + LEVEL_SIZE_MAX * x;

            //Get tile index
            byte_t tile_index;
            seek(0x801F + i, true);
            if (!readAll(tile_index)) {
                error = "Error reading tile terrain\n" + error;
                return;
            }

            //Get tile flags
            unsigned short tile_flags;
            seek(0x001F + (i * 2), true);
            if (!readAll(tile_flags)) {
                error = "Error reading tile flags\n" + error;
                return;
            }

            //Setup tile
            TilePrototype tile;
            tile.index = tile_index;
            switch (tile_flags) {
                default: //Unknown
                    error = "Unknown tile flags " + std::to_string(tile_flags) + " detected at " + std::to_string(tile_index);
                    return;
                case 0x0001: //Free
                    tile.isPassable = true;
                    break;
                case 0x0002: //Water
                    tile.isWater = true;
                    tile.isImmutable = true;
                    break;
                case 0x0008: //Shore
                    tile.isPassable = true;
                    tile.isShore = true;
                    tile.isImmutable = true;
                    break;
                case 0x0011: //Blocked
                    tile.isImmutable = true;
                    break;
                case 0x0021: //Ore
                    tile.isPassable = true;
                    tile.ore = MONEY_PER_TILE;
                    break;
                case 0x0041: //Sand
                    tile.isPassable = true;
                    tile.isSand = true;
                    break;
                case 0x0061: //Unknown
                    tile.isPassable = true;
                    break;
            }
            tiles.emplace_back(tile);
        }
    }
}

void AssetLevelGame::entities(std::vector<EntityPrototype>& entities) {
    seek(0xC02B, true);
}

void AssetLevelGame::players(std::vector<PlayerPrototype>& players) {
    seek(0xF657, true);
    for (int i = 0; i < 6; ++i) {
        //Read index
        byte_t index;
        if (!readAll(index)) {
            error = "Error reading player index\n" + error;
            return;
        }
        //Skip some data
        seek(0x4BC);
        //Skip 2 unknown ints
        seek(4 * 2);
        //Read side
        unsigned int side;
        if (!readAll(side)) {
            error = "Error reading player side\n" + error;
            return;
        }
        //Read enemies
        unsigned int enemies;
        if (!readAll(enemies)) {
            error = "Error reading player enemies\n" + error;
            return;
        }
        //Skip some data
        seek(0x2B);
        //Read money
        unsigned int money;
        if (!readAll(money)) {
            error = "Error reading player attitude\n" + error;
            return;
        }
        //Skip 2 unknown ints
        seek(4 * 2);
        //Skip some data
        seek(0x450);

        //Create prototype
        PlayerPrototype player;
        player.id = side;
        player.enemies = enemies;
        player.money = money;
        players.emplace_back(player);
    }
}