//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_STATIC_OBJECT_H
#define OPENE2140_STATIC_OBJECT_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "engine/simulation/entity.h"

/**
 * Static object with simple image
 */
CLASS_ENTITY(Entity, Object, ImageComponent)
public:
    Object();

    void draw() override;
};

#endif //OPENE2140_STATIC_OBJECT_H
