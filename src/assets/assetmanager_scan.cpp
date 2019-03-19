//
// Created by Ion Agorria on 18/03/19
//
#include "core/common.h"
#include "core/utils.h"
#include "assets/assetmanager.h"

bool AssetManager::scanContainer(const std::string& path, const std::string& name) {
    std::string type = "";

    //Try to load it as dir
    int count = scanContainerDir(path, name);
    if (0 <= count) {
        type = "Directory";
    } else if (error.empty()) {
        //Try to load it as WD
        count = scanContainerWD(path, name);
        if (0 <= count) {
            type = "WD file";
        }
    }

    //If loaded then save it
    if (0 <= count && error.empty()) {
        log->debug("Loaded '{0}' as {1} with {2} assets totalling {3}", name, type, count, assetsCount);
        return true;
    } else {
        return false;
    }
}

int AssetManager::scanContainerWD(const std::string& path, const std::string& name) {
    //Create file to be common between assets created from this container file
    std::shared_ptr<File> file = std::make_shared<File>();
    if (!file->fromPath(path + name + ".WD")) {
        log->debug("Error opening file: '{0}.WD' '{1}'", name, file->getError());
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
    std::unique_ptr<byteArray> namesBlock = Utils::createBuffer(static_cast<size_t>(namesBlockSize));
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
            byte& c = namesBlock[nameIndex];
            if (c == '\0') break;
            recordName += c;
        }

        //Check if name is empty
        if (recordName.empty()) {
            error = "Name is empty at file record " + std::to_string(recordIndex);
            return -1;
        }

        //Create an asset now that we know the name and file offset/size, the file pointer is shared with each asset
        std::shared_ptr<Asset> asset = std::make_shared<Asset>(name + '/' + recordName, file, record.fileOffset, record.fileSize);
        if (!addAsset(asset)) {
            return -1;
        }
        count++;
    }

    return count;
}

int AssetManager::scanContainerDir(const std::string& path, const std::string& name) {
    //Set first dir
    std::list<std::string> paths;
    paths.push_back(name);

    //Do until all directories are loaded
    int count = 0;
    while (!paths.empty()) {
        //Get and pop current path
        std::string current = paths.front();
        paths.pop_front();

        //List directory or load asset
        std::list<std::string> content;
        //log->debug("- {0}", current);
        if (Utils::listDirectory(path + current, content)) {
            for (std::string& dirName : content) {
                //Append current path + name of directory content to paths
                dirName = current + DIR_SEP + dirName;
                paths.push_back(dirName);
            }
        } else {
            //Check if failed path is the container base path because means that is invalid or doesn't exist
            if (current == name) {
                return -1;
            }

            //Is not a directory or is not valid, try to load as file
            std::unique_ptr<File> file = std::make_unique<File>();
            if (file->fromPath(path + current)) {
                std::shared_ptr<Asset> asset = std::make_shared<Asset>(current, std::move(file), 0, 0);
                if (addAsset(asset)) {
                    count++;
                } else {
                    error = "scanContainerDir couldn't add asset\n" + error;
                    return -1;
                }
            } else {
                error = "Error opening file: '" + current + "'\n" + file->getError();
                return -1;
            }
        }
    }

    return count;
}
