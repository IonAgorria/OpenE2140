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
     * Constructs a new vector
     *
     * @param v value for all
     */
    Vector2(int v);

    /**
     * Constructs a new vector from another
     *
     * @param vector2 to copy
     */
    Vector2(const Vector2& vector2);

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
    bool operator==(const Vector2& vector2) const;

    /**
     * Compares 2 vectors if they are not equal
     *
     * @param vector2
     * @return if not equal
     */
    bool operator!=(const Vector2& vector2) const;

    /**
     * Adds vector values to this vector
     *
     * @param vector2
     */
    void operator+=(const Vector2& vector2);

    /**
     * Subtract vector values to this vector
     *
     * @param vector2
     */
    void operator-=(Vector2 const &vector2);

    /**
     * Multiply vector values with this vector values
     *
     * @param vector2
     */
    void operator*=(Vector2 const &vector2);

    /**
     * Divides vector values with this vector values
     *
     * @param vector2
     */
    void operator/=(Vector2 const &vector2);

    /**
     * Adds vector values to this vector
     *
     * @param vector2
     * @return resulting vector
     */
    Vector2 operator+(Vector2 const &vector2) const;

    /**
     * Subtract vector values to this vector
     *
     * @param vector2
     * @return resulting vector
     */
    Vector2 operator-(Vector2 const &vector2) const;

    /**
     * Multiply vector values with this vector values
     *
     * @param vector2
     * @return resulting vector
     */
    Vector2 operator*(Vector2 const &vector2) const;

    /**
     * Divides vector values with this vector values
     *
     * @param vector2
     * @return resulting vector
     */
    Vector2 operator/(Vector2 const &vector2) const;

    /**
     * @return true if vector is not zero
     */
    operator bool() const;

    /**
     * @return true if vector x and y is 0
     */
    bool zero() const;

    /**
     * @return string version of this vector
     */
    std::string toString() const;

    /**
     * Sets vector components
     *
     * @param x value
     * @param y value
     */
    void set(int x, int y);

    /**
     * Sets vector components
     *
     * @param vector2 to get values
     */
    void set(const Vector2& vector2);
};


#endif //OPENE2140_VECTOR2_H
