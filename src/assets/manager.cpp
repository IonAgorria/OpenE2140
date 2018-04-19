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

const std::string Manager::loadContainer(const std::string& path, const std::string& name) {
    std::unique_ptr<Container> container; //Use reset() to workaround base type not accepting derived constructors
    std::string type = "";

    //Try to load it as dir
    container.reset(static_cast<Container*>(new ContainerDir(path, name)));
    if (container->load(log)) {
        type = "directory";
    } else {
        //Try to load it as WD
        container.reset(static_cast<Container*>(new ContainerWD(path, name)));
        if (container->load(log)) {
            type = "file";
        }
    }

    //If loaded then save it
    if (!type.empty()) {
        log->debug("{0} contains {1} assets", name, container->count());
        containers[name] = std::move(container);
    }

    return type;
}

bool Manager::loadContainers() {
    for (std::string name : GAME_ASSETS_NAMES) {
        const std::string type = loadContainer(Utils::getInstallPath() + GAME_ASSETS_DIR + DIR_SEP, name);
        if (type.empty()) {
            log->error("Error loading: {0}", name);
            return false;
        } else {
            log->debug("Loaded as {1}: {0}", name, type);
        }
    }
    return true;
}

std::shared_ptr<Asset> Manager::getAsset(const std::string& path) {
    std::shared_ptr<Asset> asset;
    asset.get();
    for (auto& container : containers) {
        std::shared_ptr<Asset> containerAsset = container.second->getAsset(path);
        if (containerAsset) {
            asset = containerAsset;
            break;
        }
    }
    return asset;
}
