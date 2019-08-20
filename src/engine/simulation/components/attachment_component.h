//
// Created by Ion Agorria on 12/07/19
//
#ifndef OPENE2140_ATTACHMENT_COMPONENT_H
#define OPENE2140_ATTACHMENT_COMPONENT_H

#include <memory>
#include <vector>
#include "engine/simulation/components/component.h"

class Entity;
class Faction;

/**
 * Allows entities to be attached to this entity
 */
CLASS_COMPONENT(Entity, AttachmentComponent)
protected:
    /**
     * Faction for this entity
     */
    std::vector<std::shared_ptr<Entity>> attached;

public:
    /**
     * @return attached entities
     */
    const std::vector<std::shared_ptr<Entity>>& getAttached() const;

    /**
     * Attaches entity to this entity which the component belongs
     */
    void attachEntity(const std::shared_ptr<Entity>& entity);

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
