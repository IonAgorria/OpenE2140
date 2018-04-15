//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_CONTAINER_H
#define OPENE2140_CONTAINER_H

#include <map>
#include "asset.h"

/**
 * A container stores different assets that can be accessed with path
 */
class Container {
private:
    /**
     * Path of this container
     */
    std::string path;

    /**
     * Contains all assets in this container
     */
    std::map<std::string, Asset> assets{};
public:
    /**
     * Container constructor
     */
    explicit Container(const std::string& path);

    /**
     * Container destructor
     */
    virtual ~Container() = default;

    /**
     * Disable copy constructor
     */
    Container(const Container& other) = delete;

    /**
     * Disable copy operator
     */
    void operator=(Container const &other) = delete;

    /**
     * Load container contents
     */
    virtual bool load() = 0;

    /**
     * @return the path of container
     */
    const std::string& getPath();

};

#endif //OPENE2140_CONTAINER_H
