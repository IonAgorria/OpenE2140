//
// Created by Ion Agorria on 8/04/18
//
#include "core/config.h"
#include "core/utils.h"
#include "core/io/log.h"
#include "manager.h"
#include "assetpalette.h"
#include "assetimage.h"
#include <forward_list>

Manager::Manager() {
    log = Log::get("Assets");
    assetsCount = 0;
}

Manager::~Manager() {
    clearAssets();
}

bool Manager::addAsset(std::shared_ptr<Asset> asset) {
    if (!asset) {
        log->warn("Asset to add is null");
        return false;
    }
    const std::string& path = asset->getPath();
    if (assets[path]) {
        log->warn("Asset already present: '{0}'", path);
        return false;
    }
    assets[path] = asset;
    assetsCount++;
    return true;
}

bool Manager::removeAsset(const asset_path& path) {
    if (assets.erase(path) == 0) {
        log->warn("Asset is not present: '{0}'", path);
        return false;
    }
    assetsCount--;
    return true;
}

std::shared_ptr<Asset> Manager::getAsset(const asset_path& path) {
    return assets[path];
}

int Manager::getAssetsCount() {
    return assetsCount;
}

void Manager::clearAssets() {
    //Delete all stored assets
    assets.clear();
    assetsCount = 0;
}

bool Manager::initManager() {
    //Scan assets from containers
    std::string assetDir = std::string(GAME_ASSETS_DIR) + DIR_SEP;
    std::list<std::string> assetDirPaths = {
            assetDir,                                                 //Current directory
            Utils::getInstallPath() + assetDir,                       //Installation directory
            Utils::getParentPath(Utils::getInstallPath()) + assetDir, //Parent of installation directory
    };
    for (std::string name : GAME_ASSET_CONTAINER_NAMES) {
        log->debug("Loading '{0}'", name);
        bool found = false;
        for (std::string path : assetDirPaths) {
            if (scanContainer(path, name)) {
                found = true;
                break;
            }
        }

        //Nothing was found
        if (!found) {
            Utils::showErrorDialog(
                    "Error loading game data for " + name + " directory or " + name + ".WD file\n",
                    //"Check if game files are correctly set and are accesible inside following paths: \n'" + path + "'",
                    log, false, false
            );
            return false;
        }
    }

    //Scan intermediate assets
    if (!processsIntermediates()) {
        return false;
    }

    //Print loaded assets
    //for (std::pair<asset_path, std::shared_ptr<Asset>> pair : assets) log->debug(pair.first);

    log->debug("Manager has {0} assets", assetsCount);
    return true;
}

bool Manager::scanContainer(const std::string& path, const std::string& name) {
    std::string type = "";

    //Try to load it as dir
    int count = scanContainerDir(path, name);
    if (0 <= count) {
        type = "Directory";
    } else {
        //Try to load it as WD
        count = scanContainerWD(path, name);
        if (0 <= count) {
            type = "WD file";
        }
    }

    //If loaded then save it
    if (0 <= count) {
        log->debug("Loaded '{0}' as {1} with {2} assets totalling {3}", name, type, count, assetsCount);
        return true;
    } else {
        return false;
    }
}

int Manager::scanContainerWD(const std::string& path, const std::string& name) {
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
    std::string error = file->getError();
    if (amount != sizeof(unsigned int) || !error.empty()) {
        log->error("Error reading file record count {0}", error);
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
        log->error("Error reading names block size {0}", error);
        return -1;
    }

    //Now get the names block
    std::unique_ptr<byteArray> namesBlock = Utils::createBuffer(static_cast<size_t>(namesBlockSize));
    amount = file->read(namesBlock.get(), namesBlockSize);
    error = file->getError();
    if (amount != namesBlockSize || !error.empty()) {
        log->error("Error reading names block {0}", error);
        return -1;
    }
    if (namesBlock[namesBlockSize - 1] != '\0') {
        log->error("Names block doesn't end with null terminator");
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
            log->error("Error reading file record {0} {1}", recordIndex, error);
            return -1;
        }

        //Check file offset and size
        if (0 < fileSize && record.fileOffset + record.fileSize > fileSize) {
            log->error("File offset+size is out of bounds at file record {0}", recordIndex);
            return -1;
        }

        //Check name offset
        if (record.nameOffset > namesBlockSize) {
            log->error("Name offset is out of bounds at file record {0}", recordIndex);
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
            log->error("Name is empty at file record {0}", recordIndex);
            return -1;
        }

        //Create an asset now that we know the name and file offset/size, the file pointer is shared with each asset
        std::shared_ptr<Asset> asset = std::make_shared<Asset>(name + '/' + recordName, file, record.fileOffset, record.fileSize);
        if (addAsset(asset)) {
            count++;
        }
    }

    return count;
}

int Manager::scanContainerDir(const std::string& path, const std::string& name) {
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
                }
            } else {
                log->debug("Error opening file: '{0}' '{1}'", current, file->getError());
            }
        }
    }

    return count;
}

bool Manager::processsIntermediates() {
    //Counters
    long addedAssets = 0;
    long removedAssets = 0;

    //Cached assets
    std::unordered_map<asset_path, std::shared_ptr<AssetPalette>> assetPalettes;
    std::forward_list<asset_path> image_paths;
    std::forward_list<asset_path> mix_paths;

    //Iterate all assets
    for (std::pair<asset_path, std::shared_ptr<Asset>> pair : assets) {
        //Get the "extension" of asset
        asset_path assetPath = pair.first;
        std::string::size_type size = assetPath.size();
        if (4 > size) {
            continue;
        }
        std::string ext = assetPath.substr(size - 4, 4);

        //Handle special extensions
        std::shared_ptr<Asset> asset = pair.second;
        if (ext == ".PAL") {
            //Create palette asset and store it
            std::shared_ptr<AssetPalette> assetPalette = std::make_shared<AssetPalette>(
                    assetPath, asset->getFile(), asset->offset(), asset->size()
            );
            assetPalettes[assetPath] = assetPalette;
        } else if (ext == ".DAT") {
            //Store asset path for later if they are images
            bool isImage = !Utils::startsWith(assetPath, "LEVEL/DATA");
            if (isImage) {
                image_paths.push_front(assetPath);
            }
        } else if (ext == ".MIX") {
            //Check if it's not a special file
            bool isMixMax = assetPath.find("MIXMAX") != std::string::npos;
            if (!isMixMax) {
                //Store asset path for later
                mix_paths.push_front(assetPath);
            }
        }
    }

    //Iterate mix paths
    for (asset_path assetPath : mix_paths) {
        int count = processIntermediateMIX(assetPath);
        if (count == 0) {
            continue;
        } else if (count < 0) {
            return false;
        }
        addedAssets += count;

        //Remove the old asset
        if (!removeAsset(assetPath)) {
            return false;
        }
        removedAssets++;
    }

    //Iterate images paths
    for (asset_path assetPath : image_paths) {
        //Get the asset
        std::string::size_type size = assetPath.size();
        asset_path imagePath = assetPath.substr(0, size - 4);
        std::shared_ptr<Asset> asset = getAsset(assetPath);

        //Check if there is palette asset under same name
        asset_path palettePath = imagePath + ".PAL";
        std::shared_ptr<AssetPalette> assetPalette = assetPalettes[palettePath];
        if (!assetPalette) {
            log->warn("{0} doesn't have palette counterpart", assetPath);
            continue;
        }

        //Pass the image size struct to fill it
        size_t readSize = sizeof(ImageSize16);
        Vector2 imageSize;
        ImageSize16 imageSizeStruct;
        if (!asset->readAll(imageSizeStruct)) {
            log->error("Error reading '{0}' image size {1}", assetPath, asset->getError());
            return false;
        }
        imageSize.set(imageSizeStruct.width, imageSizeStruct.height);

        //Create image asset with the palette indexes data and store it
        std::shared_ptr<AssetImage> assetImage = std::make_shared<AssetImage>(
                imagePath, asset->getFile(), asset->offset() + readSize, asset->size() - readSize, imageSize, assetPalette
        );
        if (!addAsset(assetImage)) {
            return false;
        }
        addedAssets++;

        //Remove the old assets
        if (!removeAsset(assetPath) || !removeAsset(palettePath)) {
            return false;
        }
        removedAssets += 2;
    }

    log->debug("Processed intermediate assets: added {0} removed {1}", addedAssets, removedAssets);
    return true;
}

int Manager::processIntermediateMIX(const asset_path& path) {
    std::shared_ptr<Asset> asset = getAsset(path);
    asset_path basePath = path.substr(0, path.size() - 4);
    int addedAssets = 0;

    //Verify constant
    bool match = asset->match("MIX FILE  ");
    std::string error = asset->getError();
    if (!match || !error.empty()) {
        log->error("Error reading '{0}' MIX constant {1}", path, error);
        return -1;
    }

    //Pass the header to fill it
    MIXHeader header;
    size_t readSize = sizeof(header);
    size_t amount = asset->read(&header, readSize);
    error = asset->getError();
    if (amount != readSize || !error.empty()) {
        log->error("Error reading '{0}' MIX header {1}", path, error);
        return -1;
    }

    //Verify constant
    match = asset->match("ENTRY");
    error = asset->getError();
    if (!match || !error.empty()) {
        log->error("Error reading '{0}' MIX constant {1}", path, error);
        return -1;
    }

    //Read the stream positions
    std::vector<unsigned int> streamPositions;
    unsigned int streamEntry;
    readSize = sizeof(streamEntry);
    for (unsigned int i = 0; i < header.streamsCount; ++i) {
        amount = asset->read(&streamEntry, readSize);
        error = asset->getError();
        if (amount != readSize || !error.empty()) {
            log->error("Error reading '{0}' MIX stream {1} position {2}", path, i, error);
            return -1;
        }
        streamPositions.push_back(header.streamsOffset + streamEntry);
    }

    //Verify constant
    match = asset->match(" PAL ");
    error = asset->getError();
    if (!match || !error.empty()) {
        log->error("Error reading '{0}' MIX constant {1}", path, error);
        return -1;
    }

    //Read palettes
    std::vector<std::shared_ptr<AssetPalette>> palettes;
    size_t paletteSize = PALETTE_SIZE;
    for (unsigned int i = 0; i < header.palettesCount; ++i) {
        //Create palette from MIX, path is not really used but it's nice to have
        asset_path palettePath = basePath + "/" + std::to_string(i) + ".PAL";
        std::shared_ptr<AssetPalette> assetPalette = std::make_shared<AssetPalette>(
                palettePath, asset->getFile(), asset->tell(), paletteSize
        );

        //Jump to next palette
        long result = asset->seek(paletteSize);
        error = asset->getError();
        if (result < 0 || !error.empty()) {
            log->error("Error reading '{0}' MIX palette {1} when seeking {2}", path, i, error);
            return -1;
        }

        //Store this palette asset
        palettes.push_back(assetPalette);
    }

    //Check if there is stream data as DATA will be absent if no stream is present
    if (0 < header.streamsCount) {
        //Verify constant
        match = asset->match("DATA ");
        error = asset->getError();
        if (!match || !error.empty()) {
            log->error("Error reading '{0}' MIX constant {1}", path, error);
            return -1;
        }

        //Read streams
        for (unsigned int i = 0; i < streamPositions.size(); ++i) {
            //Calculate start-end-size of stream
            unsigned int start = streamPositions[i];
            unsigned int end;
            if (i < header.streamsCount) {
                end = streamPositions[i + 1];
            } else {
                end = static_cast<unsigned int>(asset->size());
            }
            size_t streamSize = end - start;

            //Attempt to detect type of stream, first we need to seek 2 unsigned shorts
            long result = asset->seek(start + sizeof(unsigned short) + sizeof(unsigned short), true);
            error = asset->getError();
            if (result < 0 || !error.empty()) {
                log->error("Error reading '{0}' MIX stream {1} when seeking {2}", path, i, error);
                return -1;
            }

            //Read 5th byte, this hack serves to detect stream type
            byte streamType;
            readSize = sizeof(streamType);
            amount = asset->read(&streamType, readSize);
            error = asset->getError();
            if (amount != readSize || !error.empty()) {
                log->error("Error reading '{0}' MIX stream {1} type {2}", path, i, error);
                return -1;
            }

            //Go back to position
            result = asset->seek(start, true);
            error = asset->getError();
            if (result < 0 || !error.empty()) {
                log->error("Error reading '{0}' MIX stream {1} when seeking {2}", path, i, error);
                return -1;
            }

            //Used to create asset
            std::shared_ptr<File> assetFile = asset->getFile();
            std::shared_ptr<AssetPalette> imagePalette = nullptr;
            Vector2 imageSize;
            bool isImageStream = false;
            //Handle the type
            switch (streamType) {
                case TYPE_IMAGE_8_INDEXED: {
                    isImageStream = true;
                    //Get image size
                    ImageSize16 imageSizeStruct;
                    if (!asset->readAll(imageSizeStruct)) {
                        log->error("Error reading '{0}' MIX stream {1} image size {2}", path, i, asset->getError());
                        return -1;
                    }
                    start += sizeof(imageSizeStruct);
                    imageSize.set(imageSizeStruct.width, imageSizeStruct.height);

                    //Skip stream type byte
                    result = asset->seek(1);
                    error = asset->getError();
                    if (result < 0 || !error.empty()) {
                        log->error("Error reading '{0}' MIX stream {1} when seeking {2}", path, i, error);
                        return -1;
                    }
                    start += 1;

                    //Get palette index
                    byte paletteIndex;
                    readSize = sizeof(paletteIndex);
                    amount = asset->read(&paletteIndex, readSize);
                    error = asset->getError();
                    if (amount != readSize || !error.empty()) {
                        log->error("Error reading '{0}' MIX stream {1} palette index {2}", path, i, error);
                        return -1;
                    }
                    start += 1;

                    //Get palette for this image
                    if (paletteIndex < header.palettesFirstIndex) {
                        log->error("Error reading '{0}' MIX stream {1} palette index {2} is lower than first index {3}",
                                   path, i, paletteIndex, header.palettesFirstIndex);
                        return -1;
                    }
                    imagePalette = palettes.at(paletteIndex - header.palettesFirstIndex);
                    break;
                }
                case TYPE_IMAGE_16_RAW: {
                    isImageStream = true;

                    //Get image size
                    ImageSize16 imageSizeStruct;
                    if (!asset->readAll(imageSizeStruct)) {
                        log->error("Error reading '{0}' MIX stream {1} image size {2}", path, i, asset->getError());
                        return -1;
                    }
                    start += sizeof(imageSizeStruct);
                    imageSize.set(imageSizeStruct.width, imageSizeStruct.height);

                    //Skip stream type byte and unknown byte
                    start += 2;

                    break;
                }
                case TYPE_IMAGE_SEGMENTED: {
                    isImageStream = true;

                    //Skip 4 unknown bytes and stream type byte
                    result = asset->seek(5);
                    error = asset->getError();
                    if (result < 0 || !error.empty()) {
                        log->error("Error reading '{0}' MIX stream {1} when seeking {2}", path, i, error);
                        return -1;
                    }
                    start += 5;

                    //Get palette index
                    byte paletteIndex;
                    readSize = sizeof(paletteIndex);
                    amount = asset->read(&paletteIndex, readSize);
                    error = asset->getError();
                    if (amount != readSize || !error.empty()) {
                        log->error("Error reading '{0}' MIX stream {1} palette index {2}", path, i, error);
                        return -1;
                    }
                    start += 1;

                    //Get palette for this image
                    if (paletteIndex < header.palettesFirstIndex) {
                        log->error("Error reading '{0}' MIX stream {1} palette index {2} is lower than first index {3}",
                                   path, i, paletteIndex, header.palettesFirstIndex);
                        return -1;
                    }
                    imagePalette = palettes.at(paletteIndex - header.palettesFirstIndex);

                    //Get segmented image header
                    SegmentedImageHeader segmentedImageHeader;
                    if (!asset->readAll(segmentedImageHeader)) {
                        log->error("Error reading '{0}' MIX stream {1} segmented image header {2}", path, i, asset->getError());
                        return -1;
                    }
                    start += sizeof(segmentedImageHeader);
                    imageSize.set(segmentedImageHeader.width, segmentedImageHeader.height);

                    //Create buffer to store image 8 bit palette indexes
                    std::shared_ptr<File> file = std::make_shared<File>();
                    if (!file->fromMemory(segmentedImageHeader.width * segmentedImageHeader.height)) {
                        log->error("Error reading '{0}' MIX stream {1} image buffer {2}", path, i, file->getError());
                        return -1;
                    }

                    break;
                }
                default: { //Unknown
                    log->warn("'{0}' MIX stream {1} is unknown type", path, i);
                    break;
                }
            }

            //Create normal or image asset from stream and add it to manager
            std::shared_ptr<Asset> assetStream;
            asset_path streamAssetPath = basePath + "/" + std::to_string(i);
            if (isImageStream) {
                log->debug("{0} {1}", path, imageSize.toString());
                assetStream = std::make_shared<AssetImage>(streamAssetPath, assetFile, start, streamSize,
                                                           imageSize, imagePalette);
            } else {
                assetStream = std::make_shared<Asset>(streamAssetPath, assetFile, start, streamSize);
            }
            if (!addAsset(assetStream)) {
                return -1;
            }
            addedAssets++;
        }
    }

    return addedAssets;
}
