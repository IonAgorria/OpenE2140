//
// Created by Ion Agorria on 30/05/19
//
#include "asset_world_level.h"

std::string AssetWorldLevel::toString() const {
    return "AssetWorldLevel(" + toStringContent() + ")";
}

std::string AssetWorldLevel::name() {
    std::string name;
    unsigned char c;
    seek(0, true);
    for (int i = 0; i < 32; ++i) {
        readAll(c);
        if (c == 0) break;
        if (c >= 0x7f) c = ' ';
        name += c;
    }
    return name;
}

asset_path_t AssetWorldLevel::tileset() {
    return nullptr;
}

void AssetWorldLevel::players(std::vector<PlayerPrototype>& entities) {
    return;
}

void AssetWorldLevel::entities(std::vector<EntityPrototype>& entities) {
    return;
}