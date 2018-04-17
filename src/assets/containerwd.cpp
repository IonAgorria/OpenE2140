//
// Created by Ion Agorria on 8/04/18
//
#include <core/log.h>
#include "core/file.h"
#include "containerwd.h"

ContainerWD::ContainerWD(const std::string& path) : Container(path + ".WD") {
}

ContainerWD::~ContainerWD() {
}

bool ContainerWD::load(const log_ptr& log) {
    //Create file to be common between assets created from this container file
    std::shared_ptr<File> file = std::make_shared<File>();
    if (!file->open(path)) {
        //log->debug("Error opening file: '{0}' '{1}'", path, file->getError());
        return false;
    }



    return true;
}
