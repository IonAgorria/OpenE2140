//
// Created by Ion Agorria on 12/07/19
//
#ifndef OPENE2140_ATTACHMENT_COMPONENT_H
#define OPENE2140_ATTACHMENT_COMPONENT_H

#include <memory>
#include <vector>
#include "component.h"
#include "engine/math/vector2.h"

class Entity;
class Faction;

struct AttachmentPoint {
    std::string code;
    std::shared_ptr<Entity> entity;
    Vector2 position;
};

/**
 * Allows entities to be attached to this entity
 */
class AttachmentComponent {
CLASS_COMPONENT(Entity, AttachmentComponent)
protected:
    /**
     * Attached entities at this entity
     */
    std::vector<AttachmentPoint> attached;

    /**
     * Offset to apply to attachments
     */
    Vector2 attachmentCenterOffset;

public:
    /**
     * Unset this flag if attachments will be updated outside component update
     */
    bool updateAttachmentOnEntityChange = true;

    /**
     * @return attachment points for entities
     */
    const std::vector<AttachmentPoint>& getAttachments() const;

    /**
     * @return attached entities
     */
    std::shared_ptr<Entity> getAttached(const std::string& code) const;

    /**
     * Attaches entity to this entity which the component belongs
     *
     * @param code
     * @param entity the entity to attach
     * @return the struct containint attachment data
     */
    AttachmentPoint& attachEntity(const std::string& code, const std::shared_ptr<Entity>& entity);

    /**
     * Detaches entity to this entity which the component belongs
     */
    void detachEntity(const std::shared_ptr<Entity>& entity);

    /**
     * Detaches all entities
     */
    void detachEntities();

    /**
     * Updates the position of attached entities
     *
     * @param angle to apply to positions
     */
    void updateAttachmentPositions(number_t angle = NUMBER_ZERO);
};

#endif //OPENE2140_ATTACHMENT_COMPONENT_H
