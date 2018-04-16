//
// Created by Ion Agorria on 8/04/18
//
#include <core/utils.h>
#include <core/config.h>
#include "containerdir.h"

ContainerDir::ContainerDir(const std::string& path) : Container(path) {
}

bool ContainerDir::load() {
    //Set first dir
    std::list<std::string> paths;
    paths.push_back(path);

    //Do until all directories are loaded
    while (!paths.empty()) {
        //Get and pop current path
        std::string current = paths.front();
        paths.pop_front();

        //List directory or load asset
        std::list<std::string> content;
        if (Utils::listDirectory(current, content)) {
            for (std::string& name : content) {
                //Append current path + name of directory content to paths
                name = current + DIR_SEP + name;
                paths.push_back(name);
            }
        } else {
            //Is not a directory or is not valid, try to load as file
            //TODO
        }
    }

    return true;
}