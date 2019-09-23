//
// Created by Ion Agorria on 25/03/18
//
#ifndef OPENE2140_VECTOR2_H
#define OPENE2140_VECTOR2_H

#include <string>
#include "engine/core/to_string.h"
#include "number.h"

/**
 * Stores 2D point
 */
class Vector2 : public IToString {
public:
    /** x value */
    int x;
    /** y value */
    int y;

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
    explicit Vector2(int v);

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
     * Adds vector values to this vector
     *
     * @param v
     */
    void operator+=(int v);

    /**
     * Subtract vector values to this vector
     *
     * @param v
     */
    void operator-=(int v);

    /**
     * Multiply vector values with this vector values
     *
     * @param v
     */
    void operator*=(int v);

    /**
     * Divides vector values with this vector values
     *
     * @param v
     */
    void operator/=(int v);

    /**
     * Adds vector values to this vector
     *
     * @param v
     * @return resulting vector
     */
    Vector2 operator+(int v) const;

    /**
     * Subtract vector values to this vector
     *
     * @param v
     * @return resulting vector
     */
    Vector2 operator-(int v) const;

    /**
     * Multiply vector values with this vector values
     *
     * @param v
     * @return resulting vector
     */
    Vector2 operator*(int v) const;

    /**
     * Divides vector values with this vector values
     *
     * @param v
     * @return resulting vector
     */
    Vector2 operator/(int v) const;

    /**
     * @return true if vector is not zero
     */
    explicit operator bool() const;

    /**
     * @return true if vector x and y is 0
     */
    bool zero() const;

    /**
     * Sets vector components
     *
     * @param v value
     */
    void set(int v);

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

    /**
     * Return the length of this vector.
     *
     * @return the length
     */
    number_t length() const;

    /**
     * Rotates the vector and stores result
     *
     * @param radians of rotation
     * @param vector2 vector to change
     * @return dest
     */
    void rotate(number_t radians, Vector2& vector2) const;

    /**
     * Sets the angle of vector and stores result
     *
     * @param radians of rotation
     * @param vector to store
     * @return dest
     */
    void setAngle(number_t radians, Vector2& vector) const;

    /**
     * Gets the angle between this and destination
     *
     * @param destination vector
     * @return angle
     */
    number_t getAngle(const Vector2& destination) const;

    /*
     * IToString
     */

    std::string toString() const override;

    std::string toStringContent() const override;
};


#endif //OPENE2140_VECTOR2_H
