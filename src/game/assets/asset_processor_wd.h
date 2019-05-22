//
// Created by Ion Agorria on 21/04/19
//
#ifndef OPENE2140_ASSET_PROCESSOR_WD_H
#define OPENE2140_ASSET_PROCESSOR_WD_H

#include "assets/asset_processor.h"

/**
 * Handles the reading of WD archives into readable assets
 */
class AssetProcessorWD: public IAssetProcessor {
private:
    /**
     * Each WD container file record struct
     */
    struct WDFileRecord {
        unsigned int fileOffset;
        unsigned int fileSize;
        unsigned int unused1;
        unsigned int unused2;
        unsigned int checkSum;
        unsigned int nameOffset;
    };

    /**
     * Scans assets from WD file container and stores in manager
     *
     * @param path to use as assets root
     * @param name of container
     * @return amount of loaded assets or -1 if error occurred
     */
    int scanContainerWD(const std::string& path, const std::string& name);

    virtual bool scanContainer(const std::string& path, const std::string& name) override;
};

#endif //OPENE2140_ASSET_PROCESSOR_WD_H
