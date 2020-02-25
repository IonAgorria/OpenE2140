//
// Created by Ion Agorria on 25/03/18
//
#include "vector2.h"

Vector2::Vector2(int x, int y) {
    this->x = x;
    this->y = y;
}

Vector2::Vector2(int v) {
    this->x = v;
    this->y = v;
}

Vector2::Vector2(const Vector2& vector2) {
    this->x = vector2.x;
    this->y = vector2.y;
}

Vector2::Vector2() {
    this->x = 0;
    this->y = 0;
}

bool Vector2::operator==(const Vector2& vector2) const {
    return x == vector2.x
        && y == vector2.y;
}

bool Vector2::operator!=(const Vector2& vector2) const {
    return x != vector2.x
        || y != vector2.y;
}

void Vector2::operator+=(Vector2 const &vector2) {
    this->x += vector2.x;
    this->y += vector2.y;
}

void Vector2::operator-=(Vector2 const &vector2) {
    this->x -= vector2.x;
    this->y -= vector2.y;
}

void Vector2::operator*=(Vector2 const &vector2) {
    this->x *= vector2.x;
    this->y *= vector2.y;
}

void Vector2::operator/=(Vector2 const &vector2) {
    this->x /= vector2.x;
    this->y /= vector2.y;
}

Vector2 Vector2::operator+(Vector2 const &vector2) const {
    return Vector2(
            this->x + vector2.x,
            this->y + vector2.y
    );
}

Vector2 Vector2::operator-(Vector2 const &vector2) const {
    return Vector2(
            this->x - vector2.x,
            this->y - vector2.y
    );
}

Vector2 Vector2::operator*(Vector2 const &vector2) const {
    return Vector2(
            this->x * vector2.x,
            this->y * vector2.y
    );
}

Vector2 Vector2::operator/(Vector2 const &vector2) const {
    return Vector2(
            this->x / vector2.x,
            this->y / vector2.y
    );
}

void Vector2::operator+=(int v) {
    this->x += v;
    this->y += v;
}

void Vector2::operator-=(int v) {
    this->x -= v;
    this->y -= v;
}

void Vector2::operator*=(int v) {
    this->x *= v;
    this->y *= v;
}

void Vector2::operator/=(int v) {
    this->x /= v;
    this->y /= v;
}

Vector2 Vector2::operator+(int v) const {
    return Vector2(
            this->x + v,
            this->y + v
    );
}

Vector2 Vector2::operator-(int v) const {
    return Vector2(
            this->x - v,
            this->y - v
    );
}

Vector2 Vector2::operator*(int v) const {
    return Vector2(
            this->x * v,
            this->y * v
    );
}

Vector2 Vector2::operator/(int v) const {
    return Vector2(
            this->x / v,
            this->y / v
    );
}

Vector2::operator bool() const {
    return 0 != x && 0 != y;
}

bool Vector2::zero() const {
    return 0 == x && 0 == y;
}

void Vector2::set(int v) {
    this->x = v;
    this->y = v;
}

void Vector2::set(int x, int y) {
    this->x = x;
    this->y = y;
}

void Vector2::set(const Vector2& vector2) {
    this->x = vector2.x;
    this->y = vector2.y;
}

std::string Vector2::toStringContent() const {
    return std::to_string(x) + ", " + std::to_string(y);
}

number_t Vector2::length() const {
    return number_sqrt(x * x + y * y);
}

number_t Vector2::distance(int x, int y) const {
    int dx = this->x - x;
    int dy = this->y - y;
    return number_sqrt(dx * dx + dy * dy);
}

number_t Vector2::distance(const Vector2& vector2) const {
    int dx = this->x - vector2.x;
    int dy = this->y - vector2.y;
    return number_sqrt(dx * dx + dy * dy);
}

unsigned int Vector2::distanceSquared(int x, int y) const {
    int dx = this->x - x;
    int dy = this->y - y;
    return dx * dx + dy * dy;
}

unsigned int Vector2::distanceSquared(const Vector2& vector2) const {
    int dx = this->x - vector2.x;
    int dy = this->y - vector2.y;
    return dx * dx + dy * dy;
}

void Vector2::rotate(number_t radians, Vector2& vector2) const {
    number_t sin = number_sin(radians);
    number_t cos = number_cos(radians);
    number_t vx = int_to_number(x);
    number_t vy = int_to_number(y);
    vector2.x = number_to_int(number_sub(number_mul(vx, cos), number_mul(vy, sin)));
    vector2.y = number_to_int(number_add(number_mul(vx, sin), number_mul(vy, cos)));
}

void Vector2::setAngle(number_t radians, Vector2& vector) const {
    vector.set(number_to_int(length()), 0);
    rotate(radians, vector);
}

number_t Vector2::getAngle(const Vector2& destination) const {
    return number_atan2(
            int_to_number(x), int_to_number(y),
            int_to_number(destination.x), int_to_number(destination.y)
    );
}

void Vector2::lerp(const Vector2& other, number_t factor, Vector2& result) const {
    result.x = number_to_int(number_add(int_to_number(x), number_mul(int_to_number(other.x - x), factor)));
    result.y = number_to_int(number_add(int_to_number(y), number_mul(int_to_number(other.y - y), factor)));
}

void Vector2::scale(number_t factor, Vector2& result) const {
    number_t invLengthScaled = number_mul(number_div(NUMBER_ONE, length()), factor);
    result.x = number_to_int(number_mul(int_to_number(x), invLengthScaled));
    result.y = number_to_int(number_mul(int_to_number(y), invLengthScaled));
}
