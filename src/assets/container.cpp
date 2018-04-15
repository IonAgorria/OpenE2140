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
