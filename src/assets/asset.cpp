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
    //Try to calculate file size if none
    if (fileSize <= 0) {
        long size = file->size() - fileOffset;
        error = file->getError();
        if (0 < size && error.empty()) {
            this->fileSize = size;
        } else {
            this->fileSize = -1;
        }
    }
}

const std::string& Asset::getPath() {
    return path;
}

std::string Asset::getError() {
    std::string copy = error;
    error = "";
    return copy;
}

long Asset::tell() {
    return position;
}

long Asset::seek(long offset, bool set) {
    long newPosition;
    if (set) {
        newPosition = offset;
    } else {
        newPosition = position + offset;
    }
    if (0 <= newPosition && newPosition < fileSize) {
        position = newPosition;
        return position;
    } else {
        error = "Position got out of bounds " + std::to_string(newPosition);
        return -1;
    }
}

long Asset::size() {
    return fileSize;
}

size_t Asset::read(void* buffer, size_t amount) {
    //Check if new position exceeds max size if there is a size
    long newPosition = position + amount;
    if (0 < fileSize && newPosition >= fileSize) {
        newPosition = fileSize;
    }

    //Place the correct position of file
    long seekResult = file->seek(fileOffset + position, true);
    std::string fileError = file->getError();
    if (seekResult < 0 || !fileError.empty()) {
        error = fileError;
        if (error.empty()) {
            error = "Error seeking file before read";
        }
        return 0;
    }

    //Read the file
    size_t read = file->read(buffer, static_cast<size_t>(newPosition - position));
    if (read == 0) {
        fileError = file->getError();
        if (!fileError.empty()) {
            error = std::move(fileError);
        }
    } else {
        //Add position if read
        if (seek(read) < 0) {
            return 0;
        }
    }
    return read;
}
