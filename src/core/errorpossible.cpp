//
// Created by Ion Agorria on 22/04/18
//
#include "errorpossible.h"

std::string ErrorPossible::getError() {
    std::string copy = error;
    error = "";
    return copy;
}