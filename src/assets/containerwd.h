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
class ContainerWD : public Container {
public:
    explicit ContainerWD(const std::string& path);
    ~ContainerWD() override;
};

#endif //OPENE2140_CONTAINERWD_H
