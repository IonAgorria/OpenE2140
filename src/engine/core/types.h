//
// Created by Ion Agorria on 2/12/18
//
#ifndef OPENE2140_TYPES_H
#define OPENE2140_TYPES_H

#include <stdint.h>
/** Byte type */
using byte = uint8_t;
using byteArray = byte[];

#include <string>
/** Asset path */
using asset_path = std::string;

/** Window ID */
using window_id = unsigned int;


/**
 * Size struct with shorts
 */
struct SSize16 {
    unsigned short width;
    unsigned short height;
};

#endif //OPENE2140_TYPES_H
