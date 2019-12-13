//
// Created by Ion Agorria on 8/04/18
//
#include "engine/core/utils.h"
#include "engine/io/file.h"
#include "asset.h"

Asset::Asset(const asset_path_t& path, const std::shared_ptr<File> file, long fileOffset, long fileSize) :
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

const asset_path_t& Asset::getPath() const {
    return path;
}

const std::shared_ptr<File> Asset::getFile() const {
    return file;
}

long Asset::offset() const {
    return fileOffset;
}

long Asset::tell() const {
    return position;
}

long Asset::seek(long offset, bool set) {
    //Get new position
    long newPosition;
    if (set) {
        newPosition = offset;
    } else {
        newPosition = position + offset;
    }

    //Keep in bounds
    if (0 > newPosition) {
        newPosition = 0;
    } else if (0 < fileSize && newPosition >= fileSize) {
        newPosition = fileSize;
    }

    //Set position
    position = newPosition;
    return position;
}

long Asset::size() const {
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

bool Asset::readAll(void* buffer, size_t size) {
    size_t amount = read(buffer, size);
    if (!error.empty()) {
        return false;
    } else if (amount != size) {
        error = "Read " + std::to_string(amount) + " of expected " + std::to_string(size);
        return false;
    }
    return true;
}

bool Asset::match(const std::string& string) {
    size_t size = string.size();
    std::unique_ptr<byte_array_t> tmp = Utils::createBuffer(size);
    size_t amount = read(tmp.get(), size);
    if (!error.empty()) {
        error = string + " " + error;
        return false;
    }
    std::string bufferString = std::string(reinterpret_cast<const char*>(tmp.get()), amount);
    if (string != bufferString) {
        error = "Mismatch '" + string + "' got '" + bufferString + "'";
        return false;
    }
    return true;
}

std::string Asset::toStringContent() const {
    return " Path: " + path
         + " Offset: " + std::to_string(fileOffset)
         + " Size: " + std::to_string(fileSize)
         + " Position: " + std::to_string(position)
        ;
}
