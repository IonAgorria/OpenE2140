//
// Created by Ion Agorria on 15/04/18
//
#ifndef OPENE2140_CONTAINERDIR_H
#define OPENE2140_CONTAINERDIR_H

#include <string>
#include "container.h"

/**
 * Container implementation for directories
 */
class ContainerDir : Container {
public:
    /**
     * Container constructor
     */
    explicit ContainerDir(const std::string& path, const std::string& name);

    /**
     * Container destructor
     */
    ~ContainerDir() override;

    /**
     * Loads dir content
     */
     bool load(const log_ptr log) override;
};

#endif //OPENE2140_CONTAINERDIR_H
