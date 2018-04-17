//
// Created by Ion Agorria on 8/04/18
//
#include "container.h"
#include <string>

Container::Container(const std::string& path) : path() {
    this->path = path;
}

const std::string& Container::getPath() {
    return path;
}

Container::~Container() {
    //Delete all stored assets
    assets.clear();
}

const Asset& Container::getAsset(const std::string& path) {
    return *assets[path];
}
