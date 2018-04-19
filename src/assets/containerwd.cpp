//
// Created by Ion Agorria on 8/04/18
//
#include <core/utils.h>
#include "core/log.h"
#include "core/file.h"
#include "containerwd.h"

ContainerWD::ContainerWD(const std::string& path, const std::string& name) : Container(path, name) {
}

ContainerWD::~ContainerWD() {
}

bool ContainerWD::load(const log_ptr log) {
    //Create file to be common between assets created from this container file
    std::shared_ptr<File> file = std::make_shared<File>();
    if (!file->fromPath(path + name + ".WD")) {
        //log->debug("Error opening file: '{0}' '{1}'", path, file->getError());
        return false;
    }
    long fileSize = file->size();

    //Read file record count
    unsigned int recordCount;
    size_t amount = file->read(&recordCount, sizeof(unsigned int));
    std::string error = file->getError();
    if (amount != sizeof(unsigned int) || !error.empty()) {
        log->error("Error reading file record count {0}", error);
        return false;
    }

    //We want to get names block before iterating records so jump to name block size
    long recordsBlockOffset = file->tell();
    size_t recordSize = sizeof(FileRecord);
    file->seek(recordSize * recordCount);

    //Read names block size
    unsigned int namesBlockSize;
    amount = file->read(&namesBlockSize, sizeof(unsigned int));
    error = file->getError();
    if (amount != sizeof(unsigned int) || !error.empty()) {
        log->error("Error reading names block size {0}", error);
        return false;
    }

    //Now get the names block
    std::unique_ptr<byte[]> namesBlock = Utils::createBuffer(static_cast<size_t>(namesBlockSize));
    amount = file->read(namesBlock.get(), namesBlockSize);
    error = file->getError();
    if (amount != namesBlockSize || !error.empty()) {
        log->error("Error reading names block {0}", error);
        return false;
    }
    if (namesBlock[namesBlockSize - 1] != '\0') {
        log->error("Names block doesn't end with null terminator");
        return false;
    }

    //Go back to records and iterate all records
    file->seek(recordsBlockOffset, true);
    for (unsigned int recordIndex = 0; recordIndex < recordCount; ++recordIndex) {
        //Pass the record to fill it
        FileRecord record;
        amount = file->read(&record, recordSize);
        error = file->getError();
        if (amount != recordSize || !error.empty()) {
            log->error("Error reading file record {0} {1}", recordIndex, error);
            return false;
        }

        //Check file offset and size
        if (0 < fileSize && record.fileOffset + record.fileSize > fileSize) {
            log->error("File offset+size is out of bounds at file record {0}", recordIndex);
            return false;
        }

        //Check name offset
        if (record.nameOffset > namesBlockSize) {
            log->error("Name offset is out of bounds at file record {0}", recordIndex);
            return false;
        }

        //Get the name in safe way (ensure index is inside block or stop if null terminator is seen)
        std::string recordName = std::string();
        for (unsigned int nameIndex = record.nameOffset; nameIndex < namesBlockSize; ++nameIndex) {
            byte& c = namesBlock[nameIndex];
            if (c == '\0') break;
            recordName += c;
        }

        //Check if name is empty
        if (recordName.empty()) {
            log->error("Name is empty at file record {0}", recordIndex);
            return false;
        }

        //Create an asset now that we know the name and file offset/size, the file pointer is shared with each asset
        std::string assetPath = name + '/' + recordName;
        std::shared_ptr<Asset> asset = std::make_shared<Asset>(assetPath, file, record.fileOffset, record.fileSize);
        if (!addAsset(assetPath, asset)) {
            log->debug("Asset already exists: '{0}'", assetPath);
        }
    }

    return true;
}
