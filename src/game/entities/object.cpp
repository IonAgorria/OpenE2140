//
// Created by Ion Agorria on 13/06/19
//
#include "object.h"

Object::Object() {
}

void Object::draw() {
    ImageComponent::draw(renderer);
}

Tree::Tree() {
}

void Tree::draw() {
    ImageComponent::draw(renderer);
}