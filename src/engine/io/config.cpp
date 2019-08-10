//
// Created by Ion Agorria on 7/07/19
//
#include "engine/core/utils.h"
#include "engine/io/file.h"
#include "engine/math/vector2.h"
#include "engine/math/rectangle.h"
#include "config.h"

Config::Config(const std::string& path): path(path) {
    file = std::make_unique<File>();
    data = config_data_t::object();
}

void Config::clear() {
    if (data.is_discarded()) {
        data = config_data_t(nullptr);
    }
    data.clear();
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
    data = config_data_t::parse(content.get(), nullptr, false);
    if (data.is_discarded()) {
        error = "Error parsing JSON";
    }
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

bool Config::getVector2(config_data_t& section, Vector2& vector) {
    if (!section.is_array()) return false;
    if (section.size() < 2) return false;
    vector.set(
        section[0].get<int>(),
        section[1].get<int>()
    );
    return true;
}

bool Config::getRectangle(config_data_t& section, Rectangle& rectangle) {
    if (!section.is_array()) return false;
    if (section.size() < 4) return false;
    rectangle.set(
        section[0].get<int>(),
        section[1].get<int>(),
        section[2].get<int>(),
        section[3].get<int>()
    );
    return true;
}

void Config::setRectangle(Rectangle& rectangle, config_data_t& section) {
    std::vector<int> boundsArray;
    section = config_data_t(boundsArray);
    section[0] = rectangle.x;
    section[1] = rectangle.y;
    section[2] = rectangle.w;
    section[3] = rectangle.h;
}

std::string Config::toString() const {
    return "Config(" + toStringContent() + ")";
}

std::string Config::toStringContent() const {
    return " Path: " + path
        ;
}
