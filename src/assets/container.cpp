//
// Created by Ion Agorria on 8/04/18
//
#include "container.h"
#include <string>
#include "core/utils.h"

Container::Container(const std::string& path, const std::string& name) :
        path(path), name(name), assetsCount(0)
{
}

Container::~Container() {
    //Delete all stored assets
    assets.clear();
}

const std::string& Container::getPath() {
    return path;
}

const std::string& Container::getName() {
    return name;
}

bool Container::addAsset(const std::string& path, std::shared_ptr<Asset> asset) {
    std::string pathInternal = Utils::toUpper(path);
    if (assets[pathInternal]) {
        return false;
    }
    assets[pathInternal] = asset;
    assetsCount++;
    return true;
}

std::shared_ptr<Asset> Container::getAsset(const std::string& path) {
    return assets[path];
}

int Container::count() {
    return assetsCount;
}
