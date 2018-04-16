//
// Created by Ion Agorria on 8/04/18
//
#include "core/config.h"
#include "core/utils.h"
#include "core/log.h"
#include "manager.h"
#include "containerwd.h"
#include "containerdir.h"

Manager::Manager() {
    log = Log::get("Assets");
}

Manager::~Manager() {
    //Delete all pointers
    for(std::pair<std::string, Container*> container : containers) {
        delete container.second;
    }
    containers.clear();
}

bool Manager::loadContainer(const std::string& path) {
    //Try first to load as WD
    Container* container = (Container*) new ContainerWD(path);
    if (container->load()) {
        containers[path] = container;
        return true;
    }
    delete container;

    //Try to load it as dir
    container = (Container*) new ContainerDir(path);
    if (container->load()) {
        containers[path] = container;
        return true;
    }
    delete container;

    //We failed
    return false;
}

bool Manager::loadContainers() {
    for (std::string name : GAME_ASSETS_NAMES) {
        if (loadContainer(Utils::getInstallPath() + GAME_ASSETS_DIR + DIR_SEP + name)) {
            log->debug("Loaded: {0}", name);
        } else {
            log->error("Error loading: {0}", name);
            return false;
        }
    }
    return true;
}

bool Manager::loadAsset(const std::string& path) {
    return false;
}

Asset Manager::getAsset(const std::string& path) {
    return {};
}
