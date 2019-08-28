//
// Created by Ion Agorria on 12/07/19
//
#ifndef OPENE2140_ATTACHMENT_COMPONENT_H
#define OPENE2140_ATTACHMENT_COMPONENT_H

#include <memory>
#include <vector>
#include "component.h"

class Entity;
class Faction;

struct AttachmentPoint {
    std::shared_ptr<Entity> entity;
    Vector2 position;
};

/**
 * Allows entities to be attached to this entity
 */
CLASS_COMPONENT(Entity, AttachmentComponent)
protected:
    /**
     * Attached entities at this entity
     */
    std::vector<AttachmentPoint> attached;

public:
    /**
     * @return attached entities
     */
    const std::vector<AttachmentPoint>& getAttached() const;

    /**
     * Attaches entity to this entity which the component belongs
     */
    AttachmentPoint& attachEntity(const std::shared_ptr<Entity>& entity);

    /**
     * Detaches entity to this entity which the component belongs
     */
    void detachEntity(const std::shared_ptr<Entity>& entity);

    /**
     * Detaches all entities
     */
    void detachEntities();
};

#endif //OPENE2140_ATTACHMENT_COMPONENT_H