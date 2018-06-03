//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_ASSET_H
#define OPENE2140_ASSET_H

#include "core/io/file.h"
#include "core/errorpossible.h"

using asset_path = std::string;

/**
 * Base asset containing any data for game
 */
class Asset : public ErrorPossible {
private:
    /**
     * Path of this assets inside manager
     */
    const asset_path path;

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
     * @param fileSize of asset data in file, 0<= for unknown/until end
     */
    Asset(const asset_path& path, const std::shared_ptr<File> file, long fileOffset, long fileSize);

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
    const asset_path& getPath() const;

    /**
     * @return this asset file
     */
    const std::shared_ptr<File> getFile() const;

    /**
     * @return this asset file offset
     */
    long offset() const;

    /**
     * Get's the current asset seeking position
     *
     * @return position
     */
    long tell() const;

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
    long size() const;

    /**
     * Reads file data to provided buffer
     *
     * @param buffer to write
     * @param amount to write on buffer, if 0 does nothing
     * @return read amount or 0 if reached end or error occurred
     */
    size_t read(void* buffer, size_t amount);

    /**
     * Reads file data to provided buffer
     *
     * @param buffer to write
     * @param size to read
     * @return true if all bytes were read or false if error or partial read occurred
     */
    bool readAll(void* buffer, size_t size);

    /**
     * Reads file data to provided object
     *
     * @param element to write
     * @return true if all bytes were read or false if error or partial read occurred
     */
    template<typename T>
    bool readAll(T& element) {
        size_t size = sizeof(T);
        size_t amount = read(&element, size);
        if (!error.empty()) {
            return false;
        } else if (amount != size) {
            error = "Read " + std::to_string(amount) + " of expected " + std::to_string(size);
            return false;
        }
        return true;
    }

    /**
     * Checks if next data to read matches the provided string
     *
     * @param string to check
     * @return true if matches
     */
    bool match(const std::string& string);

    /**
     * @return string version of this asset
     */
    virtual std::string toString() const;

    /**
     * @return string content of this asset
     */
    virtual std::string toStringContent() const;
};

#endif //OPENE2140_ASSET_H
