//
// Created by Ion Agorria on 8/04/18
//
#include "container.h"
#include <string>
#include "core/utils.h"

Container::Container(const std::string& path, const std::string& name) : path(path), name(name) {
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

void Container::addAsset(const std::string& path, std::shared_ptr<Asset> asset) {
    assets[Utils::toUpper(path)] = asset;
}

std::shared_ptr<Asset> Container::getAsset(const std::string& path) {
    std::shared_ptr<Asset> ptr = assets[path];
    return ptr;
}
