//
// Created by Ion Agorria on 8/04/18
//
#include <boost/shared_ptr.hpp>
#include "asset.h"

Asset::Asset(std::shared_ptr<File> file, long offset, long size) : file(file) {
}

bool Asset::valid() const {
    return file != nullptr;
}
