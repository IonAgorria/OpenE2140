//
// Created by Ion Agorria on 12/04/18
//
#include <SDL_rwops.h>
#include "file.h"
#include "utils.h"

File::File() {
    file = nullptr;
}

File::~File() {
    //Close open file if any
    if (file) {
        SDL_RWclose(file);
        file = nullptr;
    }
}

bool File::open(const std::string& path, const File::FileMode& mode) {
    //Check if this file is open
    if (file) {
        error = "File is already open!";
        return false;
    }

    //Convert mode enum to SDL mode string
    const char* modeChars;
    switch (mode) {
        case FileMode::Read:
            modeChars = "rb";
            break;
        case FileMode::Write:
            modeChars = "wb";
            break;
    }

    //Open file path with mode
    file = SDL_RWFromFile(path.c_str(), modeChars);

    //Get and check any errors
    std::string sdlError = Utils::checkSDLError();
    if (sdlError.empty()) {
        return true;
    } else {
        file = nullptr;
        error = sdlError;
        return false;
    }
}

long File::tell() {
    long position = SDL_RWtell(file);
    if (position < 0) {
        error = Utils::checkSDLError();
    }
    return position;
}

long File::seek(long offset, bool set) {
    long position = SDL_RWseek(file, offset, set ? RW_SEEK_SET : RW_SEEK_CUR);
    if (position < 0) {
        error = Utils::checkSDLError();
    }
    return position;
}

std::string File::getError() {
    std::string copy = error;
    error = "";
    return copy;
}

template <typename T>
size_t File::read(T& buffer, size_t amount) {
    size_t read = SDL_RWread(file, buffer, sizeof(T), amount);
    if (read == 0) {
        std::string sdlError = Utils::checkSDLError();
        if (!sdlError.empty()) {
            error = sdlError;
        }
    }
    return read;
}
