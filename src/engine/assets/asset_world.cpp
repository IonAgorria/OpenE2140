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