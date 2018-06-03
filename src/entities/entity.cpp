//
// Created by Ion Agorria on 2018/06/03
//
#include "entity.h"
#include "core/utils.h"

Entity::Entity() {
}

std::string Entity::toString() const {
    return "Entity(" + toStringContent() + ")";
}

std::string Entity::toStringContent() const {
    return " Rectangle: " + rectangle.toString()
        ;
}
