//
// Created by Ion Agorria on 19/05/19
//
#include "asset_level.h"

AssetLevel::AssetLevel(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize) :
        Asset(path, file, fileOffset, fileSize) {
}

std::string AssetLevel::toString() const {
    return "AssetLevel(" + toStringContent() + ")";
}

void AssetLevel::dimensions(Vector2& size) {
    size.set(0, 0);
}

std::string AssetLevel::name() {
    return "";
}

asset_path_t AssetLevel::tileset(size_t index) {
    return "";
}

size_t AssetLevel::tilesetSize() {
    return 0;
}

void AssetLevel::tiles(std::vector<TilePrototype>& Tiles) {
    return;
}

void AssetLevel::players(std::vector<PlayerPrototype>& entities) {
    return;
}

void AssetLevel::entities(std::vector<EntityPrototype>& entities) {
    return;
}