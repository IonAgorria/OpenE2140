//
// Created by Ion Agorria on 8/04/18
//
#include "loader.h"
#include "containerwd.h"

Loader::Loader() {
}

Loader::~Loader() {
}

bool Loader::addContainer(std::string& path) {
    Container* container = new ContainerWD(path);
    containers[path] = container;
    return true;
}

bool Loader::loadAsset(std::string& path) {
    return false;
}

void Loader::getAsset(std::string& path) {

}
