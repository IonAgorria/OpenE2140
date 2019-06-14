//
// Created by Ion Agorria on 21/04/19
//
#include <unordered_map>
#include <forward_list>

#include "io/file.h"
#include "assets/asset_manager.h"
#include "assets/asset.h"
#include "assets/asset_palette.h"
#include "assets/asset_image.h"
#include "math/vector2.h"
#include "asset_processor_wd.h"

bool AssetProcessorWD::scanContainer(const std::string& path, const std::string& name) {
    //Try base impl first
    if (IAssetProcessor::scanContainer(path, name)) {
        return true;
    }
    //Try WD container
    int count = scanContainerWD(path, name);
    return 0 <= count && error.empty();
}

int AssetProcessorWD::scanContainerWD(const std::string& path, const std::string& name) {
    //Create file to be common between assets created from this container file
    std::shared_ptr<File> file = std::make_shared<File>();
    if (!file->fromPath(path + name + ".WD")) {
        error = "Error opening file: '" + name + ".WD' '" + file->getError() + "'";
        return -1;
    }
    long fileSize = file->size();

    //Read file record count
    unsigned int recordCount;
    size_t amount = file->read(&recordCount, sizeof(unsigned int));
    error = file->getError();
    if (amount != sizeof(unsigned int) || !error.empty()) {
        error = "Error reading file record count\n" + error;
        return -1;
    }

    //We want to get names block before iterating records so jump to name block size
    long recordsBlockOffset = file->tell();
    size_t recordSize = sizeof(WDFileRecord);
    file->seek(recordSize * recordCount);

    //Read names block size
    unsigned int namesBlockSize;
    amount = file->read(&namesBlockSize, sizeof(unsigned int));
    error = file->getError();
    if (amount != sizeof(unsigned int) || !error.empty()) {
        error = "Error reading names block size " + error;
        return -1;
    }

    //Now get the names block
    std::unique_ptr<byte_array_t> namesBlock = Utils::createBuffer(static_cast<size_t>(namesBlockSize));
    amount = file->read(namesBlock.get(), namesBlockSize);
    error = file->getError();
    if (amount != namesBlockSize || !error.empty()) {
        error = "Error reading names block " + error;
        return -1;
    }
    if (namesBlock[namesBlockSize - 1] != '\0') {
        error = "Names block doesn't end with null terminator";
        return -1;
    }

    //Go back to records and iterate all records
    int count = 0;
    file->seek(recordsBlockOffset, true);
    for (unsigned int recordIndex = 0; recordIndex < recordCount; ++recordIndex) {
        //Pass the record to fill it
        WDFileRecord record;
        amount = file->read(&record, recordSize);
        error = file->getError();
        if (amount != recordSize || !error.empty()) {
            error = "Error reading file record " + std::to_string(recordIndex) + "\n" + error;
            return -1;
        }

        //Check file offset and size
        if (0 < fileSize && record.fileOffset + record.fileSize > fileSize) {
            error = "File offset+size is out of bounds at file record {0}" + std::to_string(recordIndex);
            return -1;
        }

        //Check name offset
        if (record.nameOffset > namesBlockSize) {
            error = "Name offset is out of bounds at file record " + std::to_string(recordIndex);
            return -1;
        }

        //Get the name in safe way (ensure index is inside block or stop if null terminator is seen)
        std::string recordName = std::string();
        for (unsigned int nameIndex = record.nameOffset; nameIndex < namesBlockSize; ++nameIndex) {
            byte_t& c = namesBlock[nameIndex];
            if (c == '\0') break;
            recordName += c;
        }

        //Check if name is empty
        if (recordName.empty()) {
            error = "Name is empty at file record " + std::to_string(recordIndex);
            return -1;
        }

        //Create an asset now that we know the name and file offset/size, the file pointer is shared with each asset
        std::unique_ptr<Asset> asset = std::make_unique<Asset>(name + '/' + recordName, file, record.fileOffset, record.fileSize);
        if (!manager->addAsset(std::move(asset))) {
            return -1;
        }
        count++;
    }

    return count;
}
