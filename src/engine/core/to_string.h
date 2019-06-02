//
// Created by Ion Agorria on 31/05/19
//
#ifndef OPENE2140_TO_STRING_H
#define OPENE2140_TO_STRING_H

#include <string>

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
     * @return string version of this asset
     */
    virtual std::string toString() const = 0;

    /**
     * @return string content of this asset
     */
    virtual std::string toStringContent() const = 0;
};

#endif //OPENE2140_TO_STRING_H
