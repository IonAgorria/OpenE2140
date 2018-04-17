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
     * Pointer for file containing data
     */
    const std::shared_ptr<File> file;

    /**
     * Offset where asset data start
     */
     long offset;

    /**
     * Size of asset data in file, 0 for unknown/until end
     */
     long size;

public:
    /**
     * Constructor for invalid asset
     */
     Asset() = default;

    /**
     * Asset constructor from a file
     *
     * @param offset where asset data start
     * @param size of asset data in file, 0 for unknown/until end
     */
    Asset(std::shared_ptr<File> file, long offset = 0, long size = 0);

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
     * @return if asset is valid
     */
    bool valid() const;
};

#endif //OPENE2140_ASSET_H
