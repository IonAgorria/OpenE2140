//
// Created by Ion Agorria on 8/04/18
//
#include <boost/shared_ptr.hpp>
#include "asset.h"

Asset::Asset(const std::string& path, std::shared_ptr<File> file, long offset, long size) : path(path), file(file) {
}
