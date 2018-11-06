//
// Created by Ion Agorria on 29/04/18
//
#include "palette.h"

MapPalette::MapPalette() {
}

MapPalette::MapPalette(std::unordered_map<int, ColorRGB> colors) {
    this->colors = colors;
}

bool MapPalette::getColor(int index, ColorRGB& color) {
    //Check index
    if (!checkIndex(index)) {
        error = "Index out of bounds: " + std::to_string(index);
        return false;
    }

    //Get color if there is any
    auto iter = colors.find(index);
    if (iter != colors.end()) {
        color.set(iter->second);
        return true;
    } else {
        return false;
    }
}

bool MapPalette::setColor(int index, ColorRGB& color) {
    //Check index
    if (!checkIndex(index)) {
        error = "Index out of bounds: " + std::to_string(index) + " color " + color.toString();
        return false;
    }

    //Set color
    colors[index].set(color);
    return true;
}