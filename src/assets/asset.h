//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_ASSET_H
#define OPENE2140_ASSET_H

#include "core/io/file.h"
#include "core/errorpossible.h"

/**
 * Base asset containing any data for game
 */
class Asset : public ErrorPossible {
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

    /**
     * Last occurred error
     */
    std::string error;
public:
    /**
     * Asset constructor from a file
     *
     * @param path where this asset is located
     * @param file to use when accessing data
     * @param fileOffset where asset data start in file
     * @param fileSize of asset data in file, 0 for unknown/until end
     */
    Asset(const std::string& path, const std::shared_ptr<File> file, long fileOffset, long fileSize);

    /**
     * Asset destructor
     */
    virtual ~Asset() = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Asset)

    /**
     * @return this asset path
     */
    const std::string& getPath();

    /**
     * @return the last occurred error and clears it
     */
    std::string getError();

    /**
     * Get's the current asset seeking position
     *
     * @return position
     */
    long tell();

    /**
     * Seeks the asset position
     *
     * @param offset to seek
     * @param set to use start instead of current when adding offset
     * @return position or -1 if error or couldn't seek
     */
    long seek(long offset, bool set = false);

    /**
     * Get's the current asset size
     *
     * @return file size or -1 if unknown or error occurred
     */
    long size();

    /**
     * Reads file data to provided buffer
     *
     * @param buffer to write
     * @param amount to write on buffer, if 0 does nothing
     * @return read amount or 0 if reached end or error occurred
     */
    size_t read(void* buffer, size_t amount);
};

#endif //OPENE2140_ASSET_H
