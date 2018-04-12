//
// Created by Ion Agorria on 8/04/18
//
#include "manager.h"
#include "containerwd.h"

Manager::Manager() {
}

Manager::~Manager() {
}

bool Manager::addContainer(std::string& path) {
    Container* container = new ContainerWD(path);
    containers[path] = container;
    return true;
}

bool Manager::loadAsset(std::string& path) {
    return false;
}

void Manager::getAsset(std::string& path) {

}
