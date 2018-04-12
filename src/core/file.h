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
     * Opens the file from the specified path and selected mode of opening
     *
     * @param path of file
     * @param mode for opening
     * @param error to write if a failure occurs
     * @return true on success
     */
    bool open(const std::string& path, const File::FileMode& mode, std::string& error);
};

#endif //OPENE2140_FILE_H
