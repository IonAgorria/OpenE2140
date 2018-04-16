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
    File() = default;

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
    std::string& getError();

    /**
     * Opens the file from the specified path and selected mode of opening
     *
     * @param path of file
     * @param mode for opening
     * @return true on success
     */
    bool open(const std::string& path, const File::FileMode& mode);

    /**
     * Get's the current file seeking position
     *
     * @return position
     */
    long getPosition();

    /**
     * Set's the current file seeking position
     *
     * @param position to set
     * @return position or -1 if error or couldn't seek
     */
    void setPosition(long position);

    /**
     * Moves current file seeking position forward if positive or backwards if negative
     *
     * @param position to move
     * @return position or -1 if error or couldn't seek
     */
    void movePosition(long amount);
};

#endif //OPENE2140_FILE_H
