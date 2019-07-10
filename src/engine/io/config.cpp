//
// Created by Ion Agorria on 7/07/19
//
#include "engine/core/utils.h"
#include "engine/io/file.h"
#include "config.h"

Config::Config(const std::string& path): path(path) {
    file = std::make_unique<File>();
}

void Config::read() {
    //Prepare file
    file->close();
    file->fromPath(path, File::FileMode::Read);
    error = file->getError();
    if (hasError()) return;

    //Read the content of file
    size_t fileSize = file->size() + 1;
    std::unique_ptr<byte_array_t> content = Utils::createBuffer(fileSize);
    file->read(content.get(), fileSize);
    file->close();
    error = file->getError();
    if (hasError()) return;

    //Parse the content
    data = nlohmann::json::parse(content.get());
}

void Config::write() {
    //Prepare file
    file->close();
    file->fromPath(path, File::FileMode::Write);
    error = file->getError();
    if (hasError()) return;

    //Generate content
    std::string content = data.dump();

    //Write the content to file
    file->write(content.c_str(), content.size());
    file->close();
}

std::string Config::toString() const {
    return "Config(" + toStringContent() + ")";
}

std::string Config::toStringContent() const {
    return " Path: " + path
        ;
}