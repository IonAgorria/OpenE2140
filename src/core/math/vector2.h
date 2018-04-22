//
// Created by Ion Agorria on 25/03/18
//
#ifndef OPENE2140_VECTOR2_H
#define OPENE2140_VECTOR2_H

#include <string>
#include "SDL_rect.h"

/**
 * Stores 2D point
 */
class Vector2 : public SDL_Point {
public:
    /**
     * Constructs a new vector
     *
     * @param x position
     * @param y position
     */
    Vector2(int x, int y);

    /**
     * Constructs a zero vector
     *
     * @param x position
     * @param y position
     */
    Vector2();

    /**
     * Compares 2 vectors if they are equal
     *
     * @param vector2
     * @return if equal
     */
    bool operator==(const Vector2& vector2);

    /**
     * Compares 2 vectors if they are not equal
     *
     * @param vector2
     * @return if not equal
     */
    bool operator!=(const Vector2& vector2);

    /**
     * @return true if vector is not zero
     */
    operator bool();

    /**
     * @return true if vector x and y is 0
     */
    bool zero();

    /**
     * @return string version of this vector
     */
    std::string toString();
};


#endif //OPENE2140_VECTOR2_H
