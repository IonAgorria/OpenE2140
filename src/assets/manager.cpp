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
    //Delete all stored containers
    containers.clear();
}

const std::string Manager::loadContainer(const std::string& path) {
    std::unique_ptr<Container> container; //Use reset() to workaround base type not accepting derived constructors

    //Try first to load as WD
    container.reset((Container*) new ContainerWD(path));
    if (container->load(log)) {
        containers[path] = std::move(container);
        return "file";
    }

    //Try to load it as dir
    container.reset((Container*) new ContainerDir(path));
    if (container->load(log)) {
        containers[path] = std::move(container);
        return "directory";
    }

    //We failed
    return "";
}

bool Manager::loadContainers() {
    for (std::string name : GAME_ASSETS_NAMES) {
        const std::string type = loadContainer(Utils::getInstallPath() + GAME_ASSETS_DIR + DIR_SEP + name);
        if (type.empty()) {
            log->error("Error loading: {0}", name);
            return false;
        } else {
            log->debug("Loaded as {1}: {0}", name, type);
        }
    }
    return true;
}

const Asset& Manager::getAsset(const std::string& path) {
    for (auto& container : containers) {
        const Asset& asset = container.second->getAsset(path);
        if (asset.valid()) {
            return asset;
        }
    }
    return Asset();
}
