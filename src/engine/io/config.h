//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_CONFIG_H
#define OPENE2140_CONFIG_H

#include "engine/core/macros.h"
#include "engine/core/error_possible.h"
#include "engine/core/to_string.h"
#include "engine/io/file.h"

class File;
class Vector2;
class Rectangle;

/**
 * Handles the reading and storing of game configuration files
 */
class Config : public IErrorPossible, public IToString {
protected:
    /**
     * The path which this config is located
     */
    const std::string path;

    /**
     * Pointer for file containing data
     */
    std::unique_ptr<File> file;

public:
    /**
     * Contains config data
     */
    config_data_t data;

    /**
     * Config constructor from a file
     *
     * @param path where this config is located
     */
    Config(const std::string& path);

    /**
     * Asset destructor
     */
    virtual ~Config() = default;

    /**
     * Clears config state in memory
     */
    void clear();

    /**
     * Loads config state from file into memory
     */
    void read();

    /**
     * Writes the config state from memory into file
     */
    void write();

    /**
     * Reads vector2 from config data entry
     *
     * @param section to read from
     * @param vector to write
     * @return if successful
     */
    static bool getVector2(config_data_t& section, Vector2& vector);

    /**
     * Reads rectangle from config data entry
     *
     * @param section to read from
     * @param rectangle to write
     * @return if successful
     */
    static bool getRectangle(config_data_t& section, Rectangle& rectangle);

    /**
     * Writes rectangle to config data entry
     *
     * @param rectangle to read from
     * @param section to write
     * @return if successful
     */
    static void setRectangle(Rectangle& rectangle, config_data_t& section);

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Config)

    std::string toString() const override;

    std::string toStringContent() const override;
};

#endif //OPENE2140_CONFIG_H
