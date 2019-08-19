//
// Created by Ion Agorria on 21/04/19
//
#include "engine/io/file.h"
#include "engine/core/utils.h"
#include "asset_processor.h"
#include "asset.h"
#include "asset_manager.h"

void IAssetProcessor::setManager(AssetManager* current) {
    manager = current;
}

bool IAssetProcessor::scanContainer(const std::string& path, const std::string& name) {
    int count = scanContainerDir(path, name);
    return 0 <= count && error.empty();
}

int IAssetProcessor::scanContainerDir(const std::string& path, const std::string& name) {
    //Set first dir
    std::list<std::string> paths;
    paths.push_back(name);

    //Do until all directories are loaded
    int count = 0;
    while (!paths.empty()) {
        //Get and pop current path
        std::string current = paths.front();
        paths.pop_front();

        //List directory or load asset
        std::list<std::string> content;
        //log->debug("- {0}", current);
        if (Utils::listDirectory(path + current, content)) {
            for (std::string& dirName : content) {
                //Append current path + name of directory content to paths
                dirName = current + DIR_SEP + dirName;
                paths.push_back(dirName);
            }
        } else {
            //Check if failed path is the container base path because means that is invalid or doesn't exist
            if (current == name) {
                return -1;
            }

            //Is not a directory or is not valid, try to load as file
            std::unique_ptr<File> file = std::make_unique<File>();
            if (file->fromPath(path + current)) {
                std::unique_ptr<Asset> asset = std::make_unique<Asset>(current, std::move(file), 0, 0);
                if (manager->addAsset(std::move(asset))) {
                    count++;
                } else {
                    error = "scanContainerDir couldn't add asset\n" + manager->getError();
                    return -1;
                }
            } else {
                error = "Error opening file: '" + current + "'\n" + file->getError();
                return -1;
            }
        }
    }

    return count;
}

void IAssetProcessor::processIntermediates() {
}

void IAssetProcessor::refreshAssets() {
}
