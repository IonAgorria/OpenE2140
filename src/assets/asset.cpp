//
// Created by Ion Agorria on 8/04/18
//
#include "core/utils.h"
#include "boost/shared_ptr.hpp"
#include "asset.h"

Asset::Asset(const std::string& path, std::shared_ptr<File> file, long fileOffset, long fileSize) :
        path(Utils::toInternalPath(Utils::toUpper(path))),
        file(file), fileOffset(fileOffset), fileSize(fileSize),
        position(0) {
}

const std::string& Asset::getPath() {
    return path;
}
