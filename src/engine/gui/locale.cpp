//
// Created by Ion Agorria on 23/07/19
//
#include "locale.h"
#include "engine/io/config.h"

Locale::Locale(const std::string& code, const std::string& path): code(code) {
    load(path);
}

void Locale::load(const std::string& path) {
    //Load file
    Config config(path);
    config.read();
    error = config.getError();
    if (hasError()) {
        return;
    }
    config_data_t data = config.data;

    //Check if type is correct
    if (!data.is_object()) {
        error = "Config doesn't contain object as root";
        return;
    }

    //Iterate each text and add to texts
    for (config_data_t::iterator entry = data.begin(); entry != data.end(); ++entry) {
        texts[entry.key()] = entry.value().get<std::string>();
    }
}

const std::string& Locale::getText(const std::string& key) {
    return texts[key];
}
