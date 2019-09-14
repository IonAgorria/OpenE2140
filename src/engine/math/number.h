//
// Created by Ion Agorria on 10/9/19
//
#ifndef OPENE2140_NUMBER_H
#define OPENE2140_NUMBER_H

/** Number types */
#include "libfixmath/fixmath.h"
using number_t = fix16_t;
using Number = Fix16;

/** Constants */
#define NUMBER_PI fix16_pi
#define NUMBER_PI2 (fix16_pi * 2)
#define NUMBER_ONE fix16_one
#define NUMBER_ZERO static_cast<number_t>(0)

/** Definition aliases */
#define int_to_number(X) fix16_from_int(static_cast<short>(X))
#define number_to_int(X) fix16_to_int(X)
#define float_to_number(X) fix16_from_float(X)
#define number_to_float(X) fix16_to_float(X)
#define number_add(A,B) fix16_add(A,B)
#define number_sub(A,B) fix16_sub(A,B)
#define number_mul(A,B) fix16_mul(A,B)
#define number_div(A,B) fix16_div(A,B)
#define number_floor(X) fix16_floor(X)
#define number_ceil(X) fix16_ceil(X)
#define number_abs(X) fix16_abs(X)
#define number_sin(X) fix16_sin(X)
#define number_cos(X) fix16_cos(X)

/** Utils */
number_t number_atan2(number_t x1, number_t y1, number_t x2, number_t y2);
number_t number_wrap_angle(number_t angle);

#endif //OPENE2140_NUMBER_H
