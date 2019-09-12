//
// Created by Ion Agorria on 10/9/19
//

#include "number.h"

number_t number_atan2(number_t x1, number_t y1, number_t x2, number_t y2) {
    return fix16_atan2(x2 - x1, y2 - y1);
}

number_t number_wrap_angle(number_t angle) {
    while (angle > NUMBER_PI) {
        angle = number_sub(angle, NUMBER_PI2);
    }
    while (angle < -NUMBER_PI) {
        angle = number_add(angle, NUMBER_PI2);
    }
    return angle;
}
