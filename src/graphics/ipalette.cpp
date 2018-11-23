//
// Created by Ion Agorria on 13/11/18
//

#include "color.h"
#include "ipalette.h"

bool IPalette::set(IPalette& palette) {
    for (unsigned int i = 0; i < palette.length(); ++i) {
        ColorRGBA color;
        if (!palette.getColor(i, color)) {
            return false;
        }
        if (!setColor(i,color)) {
            return false;
        }
    }
    return true;
}

bool IPalette::checkIndex(unsigned int index) {
    return index < length();
}

bool IPalette::getColor(unsigned int index, ColorRGBA& color) {
    //Check index
    if (!checkIndex(index)) {
        error = "Index out of bounds: " + std::to_string(index);
        return false;
    }

    return getColorVirtual(index, color);
}

bool IPalette::setColor(unsigned int index, ColorRGBA& color) {
    //Check index
    if (!checkIndex(index)) {
        error = "Index out of bounds: " + std::to_string(index) + " color " + color.toString();
        return false;
    }

    return setColorVirtual(index, color);
}