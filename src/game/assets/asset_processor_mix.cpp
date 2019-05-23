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
#include "core/utils.h"
#include "math/vector2.h"
#include "asset_processor_mix.h"

void AssetProcessorMIX::processIntermediates() {
    //Cached assets
    std::forward_list<asset_path> mixPaths;

    //Iterate all assets
    for (const std::pair<const asset_path, std::unique_ptr<Asset>>& pair : manager->getAssets()) {
        //Get the "extension" of asset
        asset_path assetPath = pair.first;
        std::string::size_type size = assetPath.size();
        if (4 > size) {
            continue;
        }
        std::string ext = assetPath.substr(size - 4, 4);

        //Handle special extensions
        if (ext == ".MIX") {
            //Check if it's not a special file
            //Unknown content or purpose files, doesn't seem to be image MIXes
            bool isMixMax = assetPath.find("MIXMAX") != std::string::npos;
            if (isMixMax) continue;
            //Contains network UI images
            bool isNet = assetPath.find("NET.MIX") != std::string::npos;
            if (isNet) continue;
            //Store asset path for later
            mixPaths.push_front(assetPath);
        }
    }

    //Iterate mix paths
    for (asset_path assetPath : mixPaths) {
        Asset* asset = manager->getAsset(assetPath);
        if (!asset) continue;
        int count = processIntermediateMIX(asset);
        if (!error.empty()) return;
        if (count == 0) {
            continue;
        }

        //Remove the old asset
        if (!manager->removeAsset(assetPath)) {
            error = "Couldn't remove processed asset\n" + error;
        }
        if (!error.empty()) return;
    }
}

int AssetProcessorMIX::processIntermediateMIX(Asset* asset) {
    asset_path path = asset->getPath();
    asset_path basePath = path.substr(0, path.size() - 4);
    int addedAssets = 0;

    //Verify constant
    bool match = asset->match("MIX FILE  ");
    error = asset->getError();
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
            asset_path streamAssetPath = basePath + "/" + std::to_string(i);
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

            //Read 5th byte (unknown purpose), this hack serves to detect stream type
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
            bool isImageStream = true;
            //Handle the type
            switch (streamType) {
                case TYPE_IMAGE_8_INDEXED: {
                    //Get image size
                    SSize16 imageSizeStruct;
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
                    //Get image size
                    SSize16 imageSizeStruct;
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

                    //Create memory file to store image 8 bit palette indexes and set it as asset file
                    assetFile = std::make_shared<File>();
                    unsigned int imagePixelCount = segmentedImageHeader.width * segmentedImageHeader.height;
                    assetSize = imagePixelCount;
                    if (!assetFile->fromMemory(assetSize)) {
                        error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + " image buffer " + assetFile->getError();
                        return -1;
                    }
                    assetStart = 0;

                    //Decode the data, ignore the last entry
                    const byte zero = 0;
                    unsigned int lineIndex = 0;
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

                            //Fill left padding
                            for (unsigned int j = 0; j < segment.padding; j++) {
                                if (lineIndex == 0) result = assetFile->write(&zero, 1); else
                                    result = assetFile->write(&zero, 1);
                                error = assetFile->getError();
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
                            amount = assetFile->write(segmentBuffer.get(), segment.width);
                            error = assetFile->getError();
                            if (amount != segment.width || !error.empty()) {
                                error = "Error reading '" + path + "' MIX stream " + std::to_string(i) + "segment buffer writing " + error;
                                return -1;
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
                        lineIndex++;
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
                    isImageStream = false;
                    //log->warn("'{0}' MIX stream {1} is unknown type", path, i);
                    assetStart += asset->offset();
                    break;
                }
            }

            //Create normal or image asset from stream and add it to manager
            std::unique_ptr<Asset> assetStream;
            if (isImageStream) {
                //log->debug("AssetImage: {0} type {1}", streamAssetPath, streamType);
                assetStream = std::make_unique<AssetImage>(streamAssetPath, assetFile, assetStart, assetSize,
                                                           imageSize, imagePalette);
            } else {
                assetStream = std::make_unique<Asset>(streamAssetPath, assetFile, assetStart, assetSize);
            }
            if (!manager->addAsset(std::move(assetStream))) {
                error = "Couldn't add asset";
            }
            if (!error.empty()) return -1;
            addedAssets++;
        }
    }

    return addedAssets;
}