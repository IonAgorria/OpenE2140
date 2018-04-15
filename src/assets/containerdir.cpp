//
// Created by Ion Agorria on 8/04/18
//
#include "containerdir.h"

ContainerDir::ContainerDir(const std::string& path) : Container(path) {
}

bool ContainerDir::load() {
    return true;
}
