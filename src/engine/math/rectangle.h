//
// Created by Ion Agorria on 25/03/18
//
#ifndef OPENE2140_RECTANGLE_H
#define OPENE2140_RECTANGLE_H

#include "engine/core/to_string.h"
#include "vector2.h"

class Rectangle : public IToString {
public:
    /** x value */
    int x;
    /** y value */
    int y;
    /** w value */
    int w;
    /** h value */
    int h;

    /**
     * Constructs a new rectangle
     *
     * @param x position
     * @param y position
     * @param width of rectangle
     * @param height of rectangle
     */
    Rectangle(int x, int y, int width, int height);

    /**
     * Constructs a new rectangle
     *
     * @param position of rectangle
     * @param size of rectangle
     */
    Rectangle(const Vector2& position, const Vector2& size);

    /**
     * Constructs a new rectangle
     *
     * @param position of rectangle
     */
    explicit Rectangle(const Vector2& position);

    /**
     * Constructs a new rectangle from another
     *
     * @param rectangle to copy
     */
    Rectangle(const Rectangle& rectangle);

    /**
     * Constructs a new rectangle
     *
     * @param v value for all components
     */
    explicit Rectangle(int v);

    /**
     * Constructs a new zero rectangle
     */
    Rectangle();

    /**
     * Compares 2 rectangles if they are equal
     *
     * @param rectangle
     * @return if equal
     */
    bool operator==(const Rectangle& rectangle) const;

    /**
     * Compares 2 rectangles if they are not equal
     *
     * @param rectangle
     * @return if not equal
     */
    bool operator!=(const Rectangle& rectangle) const;

    /**
     * Adds rectangle values to this rectangle
     *
     * @param rectangle
     */
    void operator+=(Rectangle const &rectangle);

    /**
     * Subtract rectangle values to this rectangle
     *
     * @param rectangle
     */
    void operator-=(Rectangle const &rectangle);

    /**
     * Multiply rectangle values with this rectangle values
     *
     * @param rectangle
     */
    void operator*=(Rectangle const &rectangle);

    /**
     * Divides rectangle values with this rectangle values
     *
     * @param rectangle
     */
    void operator/=(Rectangle const &rectangle);

    /**
     * Adds rectangle values to this rectangle
     *
     * @param rectangle
     * @return resulting rectangle
     */
    Rectangle operator+(Rectangle const &rectangle) const;

    /**
     * Subtract rectangle values to this rectangle
     *
     * @param rectangle
     * @return resulting rectangle
     */
    Rectangle operator-(Rectangle const &rectangle) const;

    /**
     * Multiply rectangle values with this rectangle values
     *
     * @param rectangle
     * @return resulting rectangle
     */
    Rectangle operator*(Rectangle const &rectangle) const;

    /**
     * Divides rectangle values with this rectangle values
     *
     * @param rectangle
     * @return resulting rectangle
     */
    Rectangle operator/(Rectangle const &rectangle) const;

    /**
     * @return true if rectangle has area
     */
    explicit operator bool() const;

    /**
     * @return true if rectangle has no area
     */
    bool empty() const;

    /**
     * @return true if rectangle x and y is 0
     */
    bool zero() const;

    /**
     * Get rectangle position
     *
     * @param vector to write to
     */
    void getPosition(Vector2& vector) const;

    /**
     * Get rectangle position
     *
     * @param rectangle to write to
     */
    void getPosition(Rectangle& rectangle) const;

    /**
     * Get rectangle size
     *
     * @param vector to write to
     */
    void getSize(Vector2& vector) const;

    /**
     * Get rectangle size
     *
     * @param rectangle to write to
     */
    void getSize(Rectangle& rectangle) const;

    /**
     * Get rectangle center
     *
     * @param vector to write to
     */
    void getCenter(Vector2& vector) const;

    /**
     * Sets rectangle components
     *
     * @param v value
     */
    void set(int v);

    /**
     * Sets rectangle components
     *
     * @param vx value
     * @param y value
     * @param width value
     * @param height value
     */
    void set(int vx, int y, int width, int height);

    /**
     * Sets rectangle components
     *
     * @param rectangle to get values
     */
    void set(const Rectangle& rectangle);

    /**
     * Sets rectangle components
     *
     * @param position of rectangle
     * @param size of rectangle
     */
    void set(const Vector2& position, const Vector2& size);

    /**
     * Sets rectangle position
     *
     * @param position of rectangle
     */
    void setPosition(const Vector2& position);

    /**
     * Sets rectangle position
     *
     * @param position of rectangle
     */
    void setPosition(const Rectangle& position);

    /**
     * Sets rectangle size
     *
     * @param size of rectangle
     */
    void setSize(const Vector2& size);

    /**
     * Sets rectangle size
     *
     * @param size of rectangle
     */
    void setSize(const Rectangle& size);

    /**
     * Sets rectangle center
     *
     * @param x value
     * @param y value
     */
    void setCenter(int x, int y);

    /**
     * Sets rectangle center
     *
     * @param center of rectangle
     */
    void setCenter(const Vector2& center);

    /**
     * Sets rectangle center and size
     *
     * @param center of rectangle
     * @param size of rectangle
     */
    void setCenter(const Vector2& center, const Vector2& size);

    /**
     * Grows the rectangle by provided width each side
     */
    void grow(int width);

    /**
     * Checks if a position is inside this rectangle
     *
     * @param x position
     * @param y position
     * @return true if position is inside
     */
    bool isInside(int x, int y) const;

    /**
     * Checks if a vector is inside this rectangle
     *
     * @param vector position
     * @return true if position is inside
     */
    bool isInside(const Vector2& vector) const;

    /**
     * Checks if a rectangle is inside this rectangle
     *
     * @param rectangle to check
     * @return true if rectangle is inside
     */
    bool isInside(const Rectangle& rectangle) const;

    /**
     * Checks if a rectangle is overlaps this rectangle
     *
     * @param rectangle to check
     * @return true if rectangle is overlapping
     */
    bool isOverlap(const Rectangle& rectangle) const;

    /**
     * Get union rectangle between provided and this rectangle
     *
     * @param rectangle to check
     * @param result where to store union rectangle
     */
    void getUnionRectangle(const Rectangle& rectangle, Rectangle& result) const;

    /**
     * Get intersection rectangle between provided and this rectangle
     *
     * @param rectangle to check
     * @param result where to store intersection rectangle
     * @return if intersection occurred
     */
    bool getIntersectRectangle(const Rectangle& rectangle, Rectangle& result) const;

    /**
     * Get intersection line between 2 points and this rectangle
     *
     * @param start of line to check, intersected line start is stored here after
     * @param end of line to check, intersected line end is stored here after
     * @return if intersection occurred
     */
    bool getIntersectLine(Vector2& start, Vector2& end) const;

    /*
     * IToString
     */

    std::string toString() const override;

    std::string toStringContent() const override;
};


#endif //OPENE2140_RECTANGLE_H
