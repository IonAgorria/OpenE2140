//
// Created by Ion Agorria on 18/03/19
//
#include <forward_list>
#include "core/common.h"
#include "core/utils.h"
#include "io/file.h"
#include "asset.h"
#include "assetpalette.h"
#include "assetimage.h"
#include "assetmanager.h"

void AssetManager::processIntermediates() {
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
        if (!error.empty()) return;
        if (count == 0) {
            continue;
        }
        addedAssets += count;

        //Remove the old asset
        if (!removeAsset(assetPath)) {
            error = "Couldn't remove processed asset\n" + error;
        }
        if (!error.empty()) return;
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
            error = "Error reading '" + assetPath + "' image size\n" + asset->getError();
            return;
        }
        imageSize.set(imageSizeStruct.width, imageSizeStruct.height);

        //Create image asset with the palette indexes data and store it
        std::shared_ptr<AssetImage> assetImage = std::make_shared<AssetImage>(
                imagePath, asset->getFile(), asset->offset() + readSize, asset->size() - readSize, imageSize, assetPalette
        );
        if (!addAsset(assetImage)) {
            addedAssets++;
        }

        //Remove the old assets
        if (!removeAsset(assetPath) || !removeAsset(palettePath)) {
            error = "Couldn't remove processed asset\n" + error;
            return;
        }
        removedAssets += 2;
    }

    log->debug("Processed intermediate assets: added {0} removed {1}", addedAssets, removedAssets);
}

int AssetManager::processIntermediateMIX(const asset_path& path) {
    std::shared_ptr<Asset> asset = getAsset(path);
    asset_path basePath = path.substr(0, path.size() - 4);
    int addedAssets = 0;

    //Verify constant
    bool match = asset->match("MIX FILE  ");
    std::string error = asset->getError();
    if (!match || !error.empty()) {
        error = "Error reading '" + path + "' MIX constant " + error;
        return -1;
    }

    //Pass the header to fill it
    MIXHeader mixHeader;
    size_t readSize = sizeof(mixHeader);
    size_t amount = asset->read(&mixHeader, readSize);
    error = asset->getError();
    if (amount != readSize || !error.empty()) {
        error = "Error reading '" + path + "' MIX header " + error;
        return -1;
    }
    //log->debug("{0} Streams: {1} Palettes: {2} Index: {3}", path, mixHeader.streamsCount, mixHeader.palettesCount, mixHeader.palettesFirstIndex);

    //Verify constant
    match = asset->match("ENTRY");
    error = asset->getError();
    if (!match || !error.empty()) {
        error = "Error reading '" + path + "' MIX constant " + error;
        return -1;
    }

    //Read the stream positions
    std::vector<unsigned int> streamPositions;
    unsigned int streamEntry;
    readSize = sizeof(streamEntry);
    for (unsigned int i = 0; i < mixHeader.streamsCount; ++i) {
        amount = asset->read(&streamEntry, readSize);
        error = asset->getError();
        if (amount != readSize || !error.empty()) {
            error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " position " + error;
            return -1;
        }
        streamPositions.push_back(mixHeader.streamsOffset + streamEntry);
    }

    //Verify constant
    match = asset->match(" PAL ");
    error = asset->getError();
    if (!match || !error.empty()) {
        error = "Error reading '" + path + "' MIX constant " + error;
        return -1;
    }
    if (asset->tell() != mixHeader.palettesOffset) {
        error = "Error reading '" + path + "' MIX palette offset ";
        error += std::to_string(asset->tell()) + " mismatch " + std::to_string(mixHeader.palettesOffset);
        return -1;
    }

    //Read palettes
    std::vector<std::shared_ptr<AssetPalette>> palettes;
    size_t paletteSize = ASSET_PALETTE_COUNT * sizeof(ColorRGB);
    for (unsigned int i = 0; i < mixHeader.palettesCount; ++i) {
        //Create palette from MIX, path is not really used but it's nice to have
        asset_path palettePath = basePath + "/" + std::to_string(i) + ".PAL";
        std::shared_ptr<AssetPalette> assetPalette = std::make_shared<AssetPalette>(
                palettePath, asset->getFile(), asset->offset() + asset->tell(), paletteSize
        );

        //Jump to next palette
        long result = asset->seek(paletteSize);
        error = asset->getError();
        if (result < 0 || !error.empty()) {
            error = "Error reading '" + path + "' MIX palette " + std::to_string(i) + " when seeking " + error;
            return -1;
        }

        //Store this palette asset
        palettes.push_back(assetPalette);
    }

    //Check if there is stream data as DATA will be absent if no stream is present
    if (0 < mixHeader.streamsCount) {
        //Verify constant
        match = asset->match("DATA ");
        error = asset->getError();
        if (!match || !error.empty()) {
            error = "Error reading '" + path + "' MIX constant " + error;
            return -1;
        }

        //Read streams
        for (unsigned int i = 0; i < streamPositions.size(); ++i) {
            //Calculate start-end-size of stream
            unsigned int streamStart = streamPositions[i];
            unsigned int streamEnd;
            if (i + 1 < mixHeader.streamsCount) {
                streamEnd = streamPositions[i + 1];
            } else {
                streamEnd = static_cast<unsigned int>(asset->size());
            }
            size_t streamSize = streamEnd - streamStart;

            //Attempt to detect type of stream, first we need to seek 2 unsigned shorts
            long result = asset->seek(streamStart + sizeof(unsigned short) + sizeof(unsigned short), true);
            error = asset->getError();
            if (result < 0 || !error.empty()) {
                error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " when seeking " + error;
                return -1;
            }

            //Read 5th byte, this hack serves to detect stream type
            byte streamType;
            readSize = sizeof(streamType);
            amount = asset->read(&streamType, readSize);
            error = asset->getError();
            if (amount != readSize || !error.empty()) {
                error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " type " + error;
                return -1;
            }

            //Go back to position
            result = asset->seek(streamStart, true);
            error = asset->getError();
            if (result < 0 || !error.empty()) {
                error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " when seeking " + error;
                return -1;
            }

            //Used to create asset
            unsigned int assetStart = streamStart;
            size_t assetSize = streamSize;
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
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " image size " + error;
                        return -1;
                    }
                    assetStart += sizeof(imageSizeStruct);
                    imageSize.set(imageSizeStruct.width, imageSizeStruct.height);

                    //Skip stream type byte
                    result = asset->seek(1);
                    error = asset->getError();
                    if (result < 0 || !error.empty()) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " when seeking " + error;
                        return -1;
                    }
                    assetStart += 1;

                    //Get palette index
                    byte paletteIndex = 0;
                    if (!asset->readAll(paletteIndex)) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " palette index " + error;
                        return -1;
                    }
                    assetStart += 1;

                    //Get palette for this image
                    if (paletteIndex < mixHeader.palettesFirstIndex) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " palette index ";
                        error += std::to_string(paletteIndex) + " is lower than first index " + std::to_string(mixHeader.palettesFirstIndex);
                        return -1;
                    }
                    imagePalette = palettes.at(paletteIndex - mixHeader.palettesFirstIndex);
                    assetSize = streamEnd - assetStart;
                    assetStart += asset->offset();
                    break;
                }
                case TYPE_IMAGE_16_RAW: {
                    isImageStream = true;

                    //Get image size
                    ImageSize16 imageSizeStruct;
                    if (!asset->readAll(imageSizeStruct)) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " image size " + asset->getError();
                        return -1;
                    }
                    assetStart += sizeof(imageSizeStruct);
                    imageSize.set(imageSizeStruct.width, imageSizeStruct.height);

                    //Skip stream type byte and unknown byte
                    assetStart += 2;
                    assetSize = streamEnd - assetStart;
                    assetStart += asset->offset();
                    break;
                }
                case TYPE_IMAGE_SEGMENTED: {
                    isImageStream = true;

                    //Skip 4 unknown bytes and stream type byte
                    result = asset->seek(5);
                    error = asset->getError();
                    if (result < 0 || !error.empty()) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " when seeking " + error;
                        return -1;
                    }

                    //Get palette index
                    byte paletteIndex;
                    readSize = sizeof(paletteIndex);
                    amount = asset->read(&paletteIndex, readSize);
                    error = asset->getError();
                    if (amount != readSize || !error.empty()) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " palette index " + error;
                        return -1;
                    }

                    //Get palette for this image
                    if (paletteIndex < mixHeader.palettesFirstIndex) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " palette index ";
                        error += std::to_string(paletteIndex) + " is lower than first index " + std::to_string(mixHeader.palettesFirstIndex);
                        return -1;
                    }
                    imagePalette = palettes.at(paletteIndex - mixHeader.palettesFirstIndex);

                    //Get segmented image header
                    SegmentedImageHeader segmentedImageHeader;
                    if (!asset->readAll(segmentedImageHeader)) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " segmented image header " + asset->getError();
                        return -1;
                    }
                    imageSize.set(segmentedImageHeader.width, segmentedImageHeader.height);

                    //Get scan line indexes
                    std::vector<unsigned short> scanLines;
                    unsigned short value;
                    for (unsigned int j = 0; j < segmentedImageHeader.scanLinesCount; ++j) {
                        if (!asset->readAll(value)) {
                            error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " scan line " + std::to_string(j) + " " + asset->getError();
                            return -1;
                        }
                        scanLines.push_back(value);
                    }

                    //Get data offsets
                    std::vector<unsigned short> dataOffsets;
                    for (unsigned int j = 0; j < segmentedImageHeader.scanLinesCount; ++j) {
                        if (!asset->readAll(value)) {
                            error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " data offset " + std::to_string(j) + " " + asset->getError();
                            return -1;
                        }
                        dataOffsets.push_back(value);
                    }

                    //Get segments
                    std::vector<SegmentedImageSegment> segments;
                    unsigned short segmentSize = sizeof(SegmentedImageSegment);
                    unsigned int segmentsAmount = segmentedImageHeader.segmentBlockSize / segmentSize;
                    for (unsigned int j = 0; j < segmentsAmount; ++j) {
                        SegmentedImageSegment segment;
                        if (!asset->readAll(segment)) {
                            error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + "segment " + std::to_string(j) + " " + asset->getError();
                            return -1;
                        }
                        segments.push_back(segment);
                    }

                    //Skip one byte
                    result = asset->seek(1);
                    error = asset->getError();
                    if (result < 0 || !error.empty()) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " when seeking " + error;
                        return -1;
                    }

                    //Set the current position as data block offset, also check the sum of offset and size is inside asset
                    long dataBlockOffset = asset->tell();
                    long dataBlockEnd = dataBlockOffset + segmentedImageHeader.dataBlockSize;
                    if (dataBlockEnd != streamEnd) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " data block end  ";
                        error += std::to_string(dataBlockEnd) + " mismatch asset end " + std::to_string(streamEnd) + " " + asset->getError();
                        return -1;
                    }

                    //Create memory file to store image 8 bit palette indexes and set it as asset file, also reset streamStart
                    assetFile = std::make_shared<File>();
                    unsigned int imagePixelCount = segmentedImageHeader.width * segmentedImageHeader.height;
                    assetSize = imagePixelCount * 2;
                    if (!assetFile->fromMemory(assetSize)) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " image buffer " + assetFile->getError();
                        return -1;
                    }
                    assetStart = 0;

                    //Decode the data, ignore the last entry
                    byte zero = 0;
                    byte one = 1;
                    for (unsigned int scanLineIndex = 0; scanLineIndex < segmentedImageHeader.scanLinesCount - 1; scanLineIndex++) {
                        //Go to data position
                        result = asset->seek(dataBlockOffset + dataOffsets.at(scanLineIndex), true);
                        error = asset->getError();
                        if (result < 0 || !error.empty()) {
                            error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " when seeking data block " + error;
                            return -1;
                        }

                        //Draw a line
                        size_t lineSize = 0;
                        unsigned short lineEnd = scanLines.at(scanLineIndex + 1) / segmentSize;
                        for (unsigned short segmentIndex = scanLines.at(scanLineIndex) / segmentSize; segmentIndex < lineEnd; segmentIndex++) {
                            SegmentedImageSegment segment = segments.at(segmentIndex);

                            //Add line size
                            lineSize += segment.padding + segment.width;

                            //Sanity check
                            if (lineSize > segmentedImageHeader.width) {
                                error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " line size ";
                                error += std::to_string(lineSize) + " is bigger than image " + std::to_string(segmentedImageHeader.width);
                                return -1;
                            }

                            //Fill left padding (palette 1 index 0)
                            for (unsigned int j = 0; j < segment.padding; j++) {
                                result = assetFile->write(&one, 1);
                                error = assetFile->getError();
                                if (result >= 0 && error.empty()) {
                                    result = assetFile->write(&zero, 1);
                                    error = assetFile->getError();
                                }
                                if (result < 0 || !error.empty()) {
                                    error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " when writing left padding " + error;
                                    return -1;
                                }
                            }

                            //Create buffer to store current segment data
                            std::unique_ptr<byteArray> segmentBuffer = Utils::createBuffer(segment.width);

                            //Read segment data to buffer
                            amount = asset->read(segmentBuffer.get(), segment.width);
                            error = asset->getError();
                            if (amount != segment.width || !error.empty()) {
                                error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + "segment buffer reading " + error;
                                return -1;
                            }

                            //Write segment data from buffer
                            for (unsigned int j = 0; j < segment.width; j++) {
                                result = assetFile->write(&zero, 1);
                                error = assetFile->getError();
                                if (result >= 0 && error.empty()) {
                                    result = assetFile->write(&segmentBuffer[j], segment.width);
                                    error = assetFile->getError();
                                }
                                if (result < 0 || !error.empty()) {
                                    error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + "segment buffer writing " + error;
                                    return -1;
                                }
                            }
                        }

                        //Fill right padding (palette + index)
                        for (unsigned int j = 0; j < segmentedImageHeader.width - lineSize; j++) {
                            result = assetFile->write(&zero, 1);
                            error = assetFile->getError();
                            if (result < 0 || !error.empty()) {
                                error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " when writing right padding " + error;
                                return -1;
                            }
                        }
                    }

                    //Check if all was written
                    if (assetFile->tell() != assetFile->size()) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i);
                        error += " not all data was written " + std::to_string(assetFile->tell()) + " vs " + std::to_string(assetFile->size());
                        return -1;
                    }

                    break;
                }
                default: { //Unknown
                    log->warn("'{0}' MIX stream {1} is unknown type", path, i);
                    assetStart += asset->offset();
                    break;
                }
            }

            //Create normal or image asset from stream and add it to manager
            std::shared_ptr<Asset> assetStream;
            asset_path streamAssetPath = basePath + "/" + std::to_string(i);
            if (isImageStream) {
                assetStream = std::make_shared<AssetImage>(streamAssetPath, assetFile, assetStart, assetSize,
                                                           imageSize, imagePalette);
            } else {
                assetStream = std::make_shared<Asset>(streamAssetPath, assetFile, assetStart, assetSize);
            }
            if (!addAsset(assetStream)) {
                error = "Couldn't add asset\n" + error;
            }
            if (!error.empty()) return -1;
        }
    }

    return addedAssets;
}