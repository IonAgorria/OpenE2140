//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_CONTAINERWD_H
#define OPENE2140_CONTAINERWD_H

#include <string>
#include "container.h"

/**
 * Container implementation for WD format files
 */
class ContainerWD : Container {
public:
    /**
     * Container constructor
     */
    explicit ContainerWD(const std::string& path, const std::string& name);

    /**
     * Container destructor
     */
    ~ContainerWD() override;

    /**
     * Loads WD file
     */
     bool load(const log_ptr log) override;
};

#endif //OPENE2140_CONTAINERWD_H
