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
     * Contains all assets in this container
     */
    std::map<std::string, Asset> assets{};
public:
    Container() = default;
    virtual ~Container() = default;
};

#endif //OPENE2140_CONTAINER_H
