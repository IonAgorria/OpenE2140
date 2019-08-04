//
// Created by Ion Agorria on 30/05/19
//

#include "engine/io/log.h"
#include "asset_level_game.h"

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
    seek(0xF653, true);
    unsigned int tileset;
    if (!readAll(tileset)) {
        error = "Error reading tileset\n" + error;
        return;
    }
    levelTilesetIndex = tileset;
}

player_id_t AssetLevelGame::getPlayerId(byte_t player) {
    //Some players have the highest bit set even when having already another bit, so set it off
    constexpr byte_t mask = BIT_MASK(7);
    if (mask < player) {
        BIT_OFF(player, mask);
    }
    return BIT_INDEX(player);
}

std::string AssetLevelGame::toString() const {
    return "AssetLevelGame(" + toStringContent() + ")";
}

void AssetLevelGame::dimensions(Vector2& size) {
    size.set(levelSize);
}

unsigned int AssetLevelGame::tileSize() {
    return 64;
}

std::string AssetLevelGame::name() {
    seek(0, true);
    std::string name = "";
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

asset_path_t AssetLevelGame::tilePath(size_t index) {
    if (index >= TILESET_MAX) {
        return "";
    }
    return "MIX/SPRT" + std::to_string(levelTilesetIndex) + "/" + std::to_string(index);
}

size_t AssetLevelGame::tilesetSize() {
    return TILESET_MAX;
}

unsigned int AssetLevelGame::tilesetIndex() {
    return levelTilesetIndex;
}

void AssetLevelGame::tiles(std::vector<TilePrototype>& tiles) {
    for (int y = 0; y < levelSize.y; ++y) {
        for (int x = 0; x < levelSize.x; ++x) {
            //This is not a typo, tiles are set this way
            int i = y + LEVEL_SIZE_MAX * x;

            //Get tile index
            byte_t tilesetIndex = 0;
            seek(0x801F + i, true);
            if (!readAll(tilesetIndex)) {
                error = "Error reading tile terrain\n" + error;
                return;
            }

            //Get tile flags
            unsigned short tile_flags = 0;
            seek(0x001F + (i * 2), true);
            if (!readAll(tile_flags)) {
                error = "Error reading tile flags\n" + error;
                return;
            }

            //Setup tile
            TilePrototype tile;
            tile.tilesetIndex = tilesetIndex;
            tile.tileFlags = 0;
            switch (tile_flags) {
                default: //Unknown
                    error = "Unknown tile flags " + std::to_string(tile_flags) + " detected at " + std::to_string(i);
                    return;
                case 0x0001: //Free
                    BIT_ON(tile.tileFlags, TILE_FLAG_PASSABLE);
                    break;
                case 0x0002: //Water
                    BIT_ON(tile.tileFlags, TILE_FLAG_PASSABLE);
                    BIT_ON(tile.tileFlags, TILE_FLAG_WATER);
                    BIT_ON(tile.tileFlags, TILE_FLAG_IMMUTABLE);
                    break;
                case 0x0008: //Shore
                    BIT_ON(tile.tileFlags, TILE_FLAG_PASSABLE);
                    BIT_ON(tile.tileFlags, TILE_FLAG_SHORE);
                    BIT_ON(tile.tileFlags, TILE_FLAG_IMMUTABLE);
                    break;
                case 0x0011: //Blocked
                    BIT_ON(tile.tileFlags, TILE_FLAG_IMMUTABLE);
                    break;
                case 0x0021: //Ore
                    BIT_ON(tile.tileFlags, TILE_FLAG_PASSABLE);
                    tile.ore = MONEY_PER_TILE;
                    break;
                case 0x0041: //Sand
                    BIT_ON(tile.tileFlags, TILE_FLAG_PASSABLE);
                    BIT_ON(tile.tileFlags, TILE_FLAG_SAND);
                    break;
                case 0x0061: //Unknown
                    BIT_ON(tile.tileFlags, TILE_FLAG_PASSABLE);
                    break;
            }
            tiles.emplace_back(tile);
        }
    }
}

void AssetLevelGame::entities(std::vector<EntityPrototype>& entities) {
    //Load unit entities
    seek(0xC02B, true);
    for (unsigned int i = 1; i <= ENTITIES_PER_SECTION; ++i) {
        //Read index
        byte_t index = 0;
        if (!readAll(index)) {
            error = "Error reading entity index\n" + error;
            return;
        }
        //Read player
        byte_t player = 0;
        if (!readAll(player)) {
            error = "Error reading entity player\n" + error;
            return;
        }
        //Read type
        byte_t type = 0;
        if (!readAll(type)) {
            error = "Error reading entity type\n" + error;
            return;
        }
        //Read unknown1
        byte_t unknown1 = 0;
        if (!readAll(unknown1)) {
            error = "Error reading entity unknown1\n" + error;
            return;
        }
        //Read x
        unsigned short x = 0;
        if (!readAll(x)) {
            error = "Error reading entity x\n" + error;
            return;
        }
        //Read y
        unsigned short y = 0;
        if (!readAll(y)) {
            error = "Error reading entity y\n" + error;
            return;
        }
        //Read flags
        unsigned short flags = 0;
        if (!readAll(flags)) {
            error = "Error reading entity flags\n" + error;
            return;
        }
        //Read unknown2
        byte_t unknown2 = 0;
        if (!readAll(unknown2)) {
            error = "Error reading entity unknown2\n" + error;
            return;
        }
        //Read disabled
        byte_t disabled = 0;
        if (!readAll(disabled)) {
            error = "Error reading entity disabled\n" + error;
            return;
        }
        //Skip this entity if index doesn't match
        if (index != i) {
            continue;
        }

        //Create entity
        EntityPrototype entity;
        entity.player = getPlayerId(player);
        entity.type.id = type;
        entity.type.kind = ENTITY_KIND_UNIT;
        entity.position.set(x, y);
        entity.direction = index % 16; //TODO convert this into game direction
        entity.exists = flags != 0;
        entity.disabled = disabled != 0;
        entities.emplace_back(entity);
    }

    //Load building entities
    seek(0xD829, true);
    for (unsigned int i = 1; i <= ENTITIES_PER_SECTION; ++i) {
        //Read index
        byte_t index = 0;
        if (!readAll(index)) {
            error = "Error reading entity index\n" + error;
            return;
        }
        //Read player
        byte_t player = 0;
        if (!readAll(player)) {
            error = "Error reading entity player\n" + error;
            return;
        }
        //Read type
        unsigned short type = 0;
        if (!readAll(type)) {
            error = "Error reading entity type\n" + error;
            return;
        }
        //Read x
        unsigned short x = 0;
        if (!readAll(x)) {
            error = "Error reading entity x\n" + error;
            return;
        }
        //Read y
        unsigned short y = 0;
        if (!readAll(y)) {
            error = "Error reading entity y\n" + error;
            return;
        }
        //Read flags
        unsigned short flags = 0;
        if (!readAll(flags)) {
            error = "Error reading entity flags\n" + error;
            return;
        }
        //Skip this entity if index doesn't match
        if (index != i) {
            continue;
        }

        //Create entity
        EntityPrototype entity;
        entity.player = getPlayerId(player);
        entity.type.id = type;
        entity.type.kind = ENTITY_KIND_BUILDING;
        entity.position.set(x, y);
        entity.direction = 0;
        entity.exists = flags != 0;
        entities.emplace_back(entity);
    }

    //Load level objects entities
    seek(0xE229, true);
    for (unsigned int i = 1; i <= ENTITIES_PER_SECTION; ++i) {
        //Read index
        byte_t index = 0;
        if (!readAll(index)) {
            error = "Error reading entity index\n" + error;
            return;
        }
        //Read unknown1
        unsigned short unknown1 = 0;
        if (!readAll(unknown1)) {
            error = "Error reading entity unknown1\n" + error;
            return;
        }
        //Read x
        unsigned short x = 0;
        if (!readAll(x)) {
            error = "Error reading entity x\n" + error;
            return;
        }
        //Read y
        unsigned short y = 0;
        if (!readAll(y)) {
            error = "Error reading entity y\n" + error;
            return;
        }
        //Read type
        unsigned short type = 0;
        if (!readAll(type)) {
            error = "Error reading entity type\n" + error;
            return;
        }
        //Skip this entity if index doesn't match
        if (index != i) {
            continue;
        }

        //Create entity
        EntityPrototype entity;
        entity.type.id = type;
        entity.type.kind = ENTITY_KIND_OBJECT;
        entity.position.set(x, y);
        entities.emplace_back(entity);
    }
}

void AssetLevelGame::players(std::vector<PlayerPrototype>& players) {
    seek(0xF657, true);
    for (unsigned int i = 0; i < PLAYERS_MAX; ++i) {
        //Read index
        byte_t index = 0;
        if (!readAll(index)) {
            error = "Error reading player index\n" + error;
            return;
        }
        if (index != i) {
            error = "Player index " + std::to_string(index) +  " doesn`t match current index " + std::to_string(i) + "\n" + error;
            return;
        }
        //Skip some data
        seek(0x4BC);
        //Skip 2 unknown ints
        seek(8);
        //Read id/mask
        unsigned int mask = 0;
        if (!readAll(mask)) {
            error = "Error reading player id/mask\n" + error;
            return;
        }

        //Read enemies
        unsigned int enemies = 0;
        if (!readAll(enemies)) {
            error = "Error reading player enemies\n" + error;
            return;
        }
        //Skip some data
        seek(3);
        //Read faction
        unsigned int faction = 0;
        if (!readAll(faction)) {
            error = "Error reading player faction\n" + error;
            return;
        }
        //Skip some data
        seek(0x24);
        //Read money
        unsigned int money;
        if (!readAll(money)) {
            error = "Error reading player money\n" + error;
            return;
        }
        //Skip 2 unknown ints
        seek(8);
        //Skip some data
        seek(0x450);

        //Create prototype
        PlayerPrototype player;
        player.id = getPlayerId(mask);
        player.enemies = enemies;
        player.faction = faction;
        player.money = money;
        players.emplace_back(player);
    }
}
