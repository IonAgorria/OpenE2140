//
// Created by Ion Agorria on 25/03/18
//
#include "rectangle.h"

Rectangle::Rectangle(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->w = width;
    this->h = height;
}

Rectangle::Rectangle(const Vector2& position, const Vector2& size) {
    this->x = position.x;
    this->y = position.y;
    this->w = size.x;
    this->h = size.y;
}

Rectangle::Rectangle(const Rectangle& rectangle) {
    this->x = rectangle.x;
    this->y = rectangle.y;
    this->w = rectangle.w;
    this->h = rectangle.h;
}

Rectangle::Rectangle() {
    this->x = 0;
    this->y = 0;
    this->w = 0;
    this->h = 0;
}

bool Rectangle::operator==(const Rectangle& rectangle) const {
    return x == rectangle.x
        && y == rectangle.y
        && w == rectangle.w
        && h == rectangle.h;
}

bool Rectangle::operator!=(const Rectangle& rectangle) const {
    return x != rectangle.x
        || y != rectangle.y
        || w != rectangle.w
        || h != rectangle.h;
}

Rectangle::operator bool() const {
    return 0 < w && 0 < h;
}

bool Rectangle::empty() const {
    return 0 >= w || 0 >= h;
}

bool Rectangle::zero() const {
    return 0 == x && 0 == y;
}

std::string Rectangle::toString() const {
    return "R(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(w) + ", " + std::to_string(h) + ")";
}

void Rectangle::set(int x, int y, int width, int height) {
    this->x = x;
    this->y = y;
    this->w = width;
    this->h = height;
}

void Rectangle::set(const Rectangle& rectangle)
{
    this->x = rectangle.x;
    this->y = rectangle.y;
    this->w = rectangle.w;
    this->h = rectangle.h;
}

void Rectangle::set(const Vector2& position, const Vector2& size) {
    this->x = position.x;
    this->y = position.y;
    this->w = size.x;
    this->h = size.y;
}

void Rectangle::setPosition(const Vector2& position) {
    this->x = position.x;
    this->y = position.y;
}

void Rectangle::setSize(const Vector2& size) {
    this->w = size.x;
    this->h = size.y;
}

void Rectangle::setCenter(int x, int y) {
    this->x = x - w / 2;
    this->y = y - h / 2;
}

void Rectangle::setCenter(const Vector2& center) {
    this->x = center.x - w / 2;
    this->y = center.y - h / 2;
}

void Rectangle::setCenter(const Vector2& center, const Vector2& size) {
    this->w = size.x;
    this->h = size.y;
    this->x = center.x - w / 2;
    this->y = center.y - h / 2;
}

void Rectangle::getCenter(Vector2& vector) const {
    vector.x = x + w / 2;
    vector.y = y + h / 2;
}

bool Rectangle::isInside(int px, int py) const {
    return x <= px && px <= x + w
        && y <= py && py <= y + h;
}

bool Rectangle::isInside(const Vector2& v) const {
    return x <= v.x && v.x <= x + w
        && y <= v.y && v.y <= y + h;
}

bool Rectangle::isInside(const Rectangle& rectangle) const {
    float xmin = rectangle.x;
    float xmax = xmin + rectangle.w;

    float ymin = rectangle.y;
    float ymax = ymin + rectangle.h;

    return xmin > x && xmin < x + w && xmax > x && xmax < x + w
        && ymin > y && ymin < y + h && ymax > y && ymax < y + h;
}

bool Rectangle::isOverlap(const Rectangle& rectangle) const {
    return x < rectangle.x + rectangle.w && rectangle.x < x + w
        && y < rectangle.y + rectangle.h && rectangle.y < y + h;

}

bool Rectangle::getIntersectRectangle(const Rectangle& rectangle, Rectangle& result) const {
    return SDL_IntersectRect(this, &rectangle, &result) == SDL_TRUE;
}

bool Rectangle::getIntersectLine(Vector2& start, Vector2& end) const {
    return SDL_IntersectRectAndLine(this, &start.x, &start.y, &end.x, &end.y);
}
