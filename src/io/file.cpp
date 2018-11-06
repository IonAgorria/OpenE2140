//
// Created by Ion Agorria on 12/04/18
//
#include "core/config.h"
#include "file.h"
#include "core/utils.h"
#include "SDL_rwops.h"

File::File() {
    file = nullptr;
}

File::~File() {
    close();
}

void File::close() {
    //Close open file if any
    if (file) {
        SDL_RWclose(file);
        file = nullptr;
    }

    //Release any memory after file close
    if (memory) {
        memory.reset();
    }
}

void File::setAnySDLError() {
    std::string sdlError = Utils::checkSDLError();
    if (!sdlError.empty()) {
        error = std::move(sdlError);
    }
}

bool File::checkInternal() {
    //Do various checks
    bool good = error.empty();
    if (good) {
        setAnySDLError();
        if (!error.empty()) {
            good = false;
        } else if (!file) {
            error = "File was not created but no error occurred";
            good = false;
        } else if (file->type == SDL_RWOPS_MEMORY && !memory) {
            error = "File was created but memory buffer is not available";
            good = false;
        }
    }

    //Close it if not good
    if (!good) {
        close();
    }

    return good;
}

bool File::fromPath(const std::string& path, const File::FileMode& mode) {
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

    //Check any errors
    return checkInternal();
}

bool File::fromMemory(const size_t size) {
    //Check if this file is open
    if (file) {
        error = "File is already open!";
        return false;
    }

    //Create memory buffer for memory file
    memory = Utils::createBuffer(size);

    //Open file path with mode
    file = SDL_RWFromMem(memory.get(), static_cast<unsigned int>(size));

    //Check any errors
    return checkInternal();
}

long File::tell() {
    long position = SDL_RWtell(file);
    if (position < 0) {
        setAnySDLError();
    }
    return position;
}

long File::seek(long offset, bool set) {
    long position = SDL_RWseek(file, offset, set ? RW_SEEK_SET : RW_SEEK_CUR);
    if (position < 0) {
        setAnySDLError();
    }
    return position;
}

long File::size() {
    long size = SDL_RWsize(file);
    if (size < 0) {
        setAnySDLError();
    }
    return size;
}

size_t File::read(void* buffer, size_t amount) {
    if (amount == 0) return 0;
    size_t read = SDL_RWread(file, buffer, 1, amount);
    if (read == 0) {
        setAnySDLError();
    }
    return read;
}

size_t File::write(void* buffer, size_t amount) {
    if (amount == 0) return 0;
    size_t written = SDL_RWwrite(file, buffer, 1, amount);
    if (written != amount) {
        setAnySDLError();
        if (error.empty()) {
            error = "No error produced but written amount is different than expected";
        }
    }
    return written;
}
