//
// Created by Ion Agorria on 12/04/18
//
#ifndef OPENE2140_FILE_H
#define OPENE2140_FILE_H

#include <string>
#include <SDL_rwops.h>

/**
 * Implements simple file I/O abstraction
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
     * File destructor
     * Cleans file handles
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
    bool open(const std::string& path, const File::FileMode& mode = FileMode::Read);

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
     * Reads filee data to provided buffer
     *
     * @tparam T type of object
     * @param buffer to write
     * @param amount of objects to write on buffer
     * @return read amount or 0 if reached end or error occurred
     */
    template <typename T>
    size_t read(T& buffer, size_t amount);
};

#endif //OPENE2140_FILE_H
