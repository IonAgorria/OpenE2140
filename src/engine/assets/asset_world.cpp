//
// Created by Ion Agorria on 19/05/19
//
#include "asset_world.h"

AssetWorld::AssetWorld(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize) :
        Asset(path, file, fileOffset, fileSize) {
}

std::string AssetWorld::toString() const {
    return "AssetWorld(" + toStringContent() + ")";
}

void AssetWorld::dimensions(Vector2& size) {
    size.set(0, 0);
}

std::string AssetWorld::name() {
    return "";
}

asset_path_t AssetWorld::tileset() {
    return nullptr;
}

void AssetWorld::tiles(std::vector<TilePrototype>& Tiles) {
    return;
}

void AssetWorld::players(std::vector<PlayerPrototype>& entities) {
    return;
}

void AssetWorld::entities(std::vector<EntityPrototype>& entities) {
    return;
}