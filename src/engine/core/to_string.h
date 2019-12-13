//
// Created by Ion Agorria on 31/05/19
//
#ifndef OPENE2140_TO_STRING_H
#define OPENE2140_TO_STRING_H

#include <string>
#include "macros.h"

/**
 * Common interface for toString methods
 */
class IToString {
public:
    /**
     * Destructor
     */
    virtual ~IToString() = default;

    /**
     * Type name needed to implement
     */
    TYPE_NAME_VIRTUAL()

    /**
     * @return string version of this object
     */
    std::string toString() const {
        return type_name() + "(" + toStringContent() + ")";
    }

    /**
     * @return string content of this asset
     */
    virtual std::string toStringContent() const {
        return "";
    }
};

#endif //OPENE2140_TO_STRING_H
