//
// Created by Ion Agorria on 25/03/18
//
#include "vector2.h"

Vector2::Vector2(int x, int y) : x(x), y(y) {
}

bool Vector2::operator==(const Vector2& vector2) {
    return x == vector2.x
        && y == vector2.y;
}

bool Vector2::operator!=(const Vector2& vector2) {
    return x != vector2.x
        || y != vector2.y;
}

Vector2::operator bool() {
    return 0 != x && 0 != y;
}

bool Vector2::zero() {
    return 0 == x && 0 == y;
}