//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_ASSET_H
#define OPENE2140_ASSET_H

#include "core/file.h"

/**
 * Base asset containing any data for game
 */
class Asset {
private:
    /**
     * Path of this assets inside of container
     */
    const std::string path;

    /**
     * Pointer for file containing data
     */
    const std::shared_ptr<File> file;

    /**
     * Offset where asset data start
     */
    long fileOffset;

    /**
     * Size of asset data in file, 0 for unknown/until end
     */
    long fileSize;

    /**
     * Current asset position reading the file
     */
    long position;
public:
    /**
     * Asset constructor from a file
     *
     * @param path where this asset is located
     * @param file to use when accessing data
     * @param fileOffset where asset data start in file
     * @param fileSize of asset data in file, 0 for unknown/until end
     */
    Asset(const std::string& path, std::shared_ptr<File> file, long fileOffset, long fileSize);

    /**
     * Asset destructor
     */
    ~Asset() = default;

    /**
     * Disable copy constructor
     */
    Asset(const Asset& other) = delete;

    /**
     * Disable copy operator
     */
    void operator=(const Asset& other) = delete;

    /**
     * @return this asset path
     */
    const std::string& getPath();
};

#endif //OPENE2140_ASSET_H
