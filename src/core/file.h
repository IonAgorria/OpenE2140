//
// Created by Ion Agorria on 12/04/18
//
#ifndef OPENE2140_FILE_H
#define OPENE2140_FILE_H

#include "config.h"
#include <string>
#include <memory>
#include "SDL_rwops.h"

/**
 * Implements simple file I/O abstraction using real files or memory
 */
class File {
private:
    /**
     * SDL internal file struct pointer
     */
    SDL_RWops* file;

    /**
     * Last occurred error
     */
    std::string error;

    /**
     * Memory buffer if file is memory based
     */
    std::unique_ptr<uint8_t[]> memory;

    /**
     * Sets error of this file if SDL has any error
     */
    void setAnySDLError();

    /**
     * Helper function for file errors, fills error var
     *
     * @return if any error occurred on creation
     */
    bool checkInternal();
public:
    /**
     * File opening modes
     */
    enum class FileMode {
        Read,
        Write
    };

    /**
     * File constructor
     */
    File();

    /**
     * File destructor, closes file
     */
    ~File();

    /**
     * Disable copy constructor
     */
    File(const File& other) = delete;

    /**
     * Disable copy operator
     */
    void operator=(const File& other) = delete;

    /**
     * Closes the file
     */
    void close();

    /**
     * @return the last occurred error and clears it
     */
    std::string getError();

    /**
     * Opens the file from the specified path and selected mode of opening
     *
     * @param path of file
     * @param mode for opening
     * @return true on success
     */
    bool fromPath(const std::string& path, const File::FileMode& mode = FileMode::Read);

    /**
     * Creates virtual file on memory stored data with specified size
     *
     * @param size of memory containing file
     * @return true on success
     */
    bool fromMemory(const size_t size);

    /**
     * Get's the current file seeking position
     *
     * @return position or -1 if error or couldn't get position
     */
    long tell();

    /**
     * Seeks the file position
     *
     * @param offset to seek
     * @param set to use start instead of current when adding offset
     * @return position or -1 if error or couldn't seek
     */
    long seek(long offset, bool set = false);

    /**
     * Get's the current file size
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

    /**
     * Writes file data from provided buffer
     *
     * @param buffer to get data from
     * @param amount to write on file, if 0 does nothing
     * @return written amount, different from amount if error occurred
     */
    size_t write(void* buffer, size_t amount);
};

#endif //OPENE2140_FILE_H
