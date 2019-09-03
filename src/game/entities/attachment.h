//
// Created by Ion Agorria on 23/06/19
//
#ifndef OPENE2140_ATTACHMENT_H
#define OPENE2140_ATTACHMENT_H

#include "engine/simulation/components/component.h"
#include "engine/simulation/components/image_component.h"
#include "src/engine/simulation/entity.h"
#include "game/components/palette_component.h"

/**
 * Entity that is created as a attachment or part of another entity
 */
CLASS_ENTITY_COMPONENTS(Entity, Attachment,
                        ImageComponent)
public:
    void simulationChanged() override;

    void update() override;

    void draw() override;
};

/**
 * Spinner object
 */
CLASS_ENTITY(Attachment, Spinner)
public:
    /**
     * Controls if spinner is clockwise or counterclockwise
     */
    bool clockwise = true;

    void simulationChanged() override;

    void update() override;
};

#endif //OPENE2140_ATTACHMENT_H
