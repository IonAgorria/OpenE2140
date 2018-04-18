//
// Created by Ion Agorria on 8/04/18
//
#include "core/utils.h"
#include "core/config.h"
#include "core/file.h"
#include "containerdir.h"

ContainerDir::ContainerDir(const std::string& path, const std::string& name) : Container(path, name) {
}

ContainerDir::~ContainerDir() {
}

bool ContainerDir::load(const log_ptr log) {
    //Set first dir
    std::list<std::string> paths;
    paths.push_back(name);

    //Do until all directories are loaded
    while (!paths.empty()) {
        //Get and pop current path
        std::string current = paths.front();
        paths.pop_front();

        //List directory or load asset
        std::list<std::string> content;
        log->debug(current);
        if (Utils::listDirectory(path + current, content)) {
            for (std::string& name : content) {
                //Append current path + name of directory content to paths
                name = current + DIR_SEP + name;
                paths.push_back(name);
            }
        } else {
            //Check if failed path is the container base path because means that is invalid or doesn't exist
            if (current == name) {
                return false;
            }

            //Is not a directory or is not valid, try to load as file
            std::unique_ptr<File> file = std::make_unique<File>();
            if (file->open(path + current)) {
                std::shared_ptr<Asset> asset = std::make_shared<Asset>(current, std::move(file), 0, 0);
                addAsset(current, asset);
            } else {
                log->debug("Error opening file: '{0}' '{1}'", current, file->getError());
            }
        }
    }

    return true;
}
