//
// Created by Ion Agorria on 25/03/18
//
#ifndef OPENE2140_RECTANGLE_H
#define OPENE2140_RECTANGLE_H

#include "SDL_rect.h"
#include "vector2.h"

class Rectangle : public SDL_Rect {
public:
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
     * Constructs a new zero rectangle
     *
     * @param x position
     * @param y position
     * @param width of rectangle
     * @param height of rectangle
     */
    Rectangle();

    /**
     * Compares 2 rectangles if they are equal
     *
     * @param rectangle
     * @return if equal
     */
    bool operator==(const Rectangle& rectangle);

    /**
     * Compares 2 rectangles if they are not equal
     *
     * @param rectangle
     * @return if not equal
     */
    bool operator!=(const Rectangle& rectangle);

    /**
     * @return true if rectangle has area
     */
    operator bool();

    /**
     * @return true if rectangle has no area
     */
    bool empty();

    /**
     * @return true if rectangle x and y is 0
     */
    bool zero();

    /**
     * @return string version of this vector
     */
    std::string toString();

    /**
     * Checks if a position is inside this rectangle
     *
     * @param x position
     * @param y position
     * @return true if position is inside
     */
    bool isInside(int x, int y);

    /**
     * Checks if a vector is inside this rectangle
     *
     * @param vector position
     * @return true if position is inside
     */
    bool isInside(const Vector2& vector);

    /**
     * Checks if a rectangle is inside this rectangle
     *
     * @param rectangle to check
     * @return true if rectangle is inside
     */
    bool isInside(const Rectangle& rectangle);

    /**
     * Checks if a rectangle is overlaps this rectangle
     *
     * @param rectangle to check
     * @return true if rectangle is overlapping
     */
    bool isOverlap(const Rectangle& rectangle);

    /**
     * Get intersection rectangle between provided and this rectangle
     *
     * @param rectangle to check
     * @param result where to store intersection rectangle
     * @return if intersection occurred
     */
    bool getIntersectRectangle(const Rectangle& rectangle, Rectangle& result);

    /**
     * Get intersection line between 2 points and this rectangle
     *
     * @param start of line to check, intersected line start is stored here after
     * @param end of line to check, intersected line end is stored here after
     * @return if intersection occurred
     */
    bool getIntersectLine(Vector2& start, Vector2& end);
};


#endif //OPENE2140_RECTANGLE_H
