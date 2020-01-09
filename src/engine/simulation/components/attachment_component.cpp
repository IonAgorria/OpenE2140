//
// Created by Ion Agorria on 20/8/19
//

#include "engine/core/macros.h"
#include "engine/io/config.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/entity.h"
#include "engine/entities/entity_config.h"
#include "attachment_component.h"

void AttachmentComponent::construction() {
}

AttachmentComponent::~AttachmentComponent() {
    //Cleanup leftovers
    if (!attached.empty()) {
        LOG_BUG("Attached entities when entity {0} was destroyed", base->getID());
        detachEntities();
    }
}

void AttachmentComponent::setup() {
}

void AttachmentComponent::simulationChanged() {
    if (base->isActive()) {
        const EntityConfig* config = base->getConfig();
        config->getVector2("attachments_center_offset", attachmentCenterOffset);
        updateAttachmentOnEntityChange = config->getData<bool>("attachments_update_on_change", true);
        config_data_t attachments = config->getData("attachments");
        if (attachments.is_object()) {
            entity_kind_t kind = config->getData("attachments_kind", 0);
            for (auto entry = attachments.begin(); entry != attachments.end(); ++entry) {
                //Read the attachment entry and get the type
                config_data_t entryValue = entry.value();
                if (!entryValue.is_object()) continue;
                std::string code = entryValue["code"].get<std::string>();

                //Create the entity and attach it
                std::shared_ptr<Entity> entity = base->getSimulation()->createEntity(kind, code, false);
                if (!entity) continue;
                AttachmentPoint& attachment = attachEntity(entry.key(), entity);

                //Set the position
                Config::getVector2(entryValue["position"], attachment.position);
            }
        }
    } else {
        detachEntities();
    }
}

void AttachmentComponent::update() {
    //Propagate update call
    for (const auto& attachment : attached) {
        Entity* entity = attachment.entity.get();
        entity->update();
    }
}

void AttachmentComponent::entityChanged() {
    if (updateAttachmentOnEntityChange) {
        updateAttachmentPositions();
    }
}

void AttachmentComponent::updateAttachmentPositions(number_t angle) {
    //Update the entities
    for (const auto& attachment : attached) {
        Vector2 position;
        //Store position or rotate it
        if (angle == NUMBER_ZERO) {
            position = attachment.position;
        } else {
            attachment.position.rotate(angle, position);
        }
        position = base->getPosition() + position + attachmentCenterOffset;
        Entity* entity = attachment.entity.get();
        entity->setPosition(position);
    }
}

const std::vector<AttachmentPoint>& AttachmentComponent::getAttachments() const {
    return attached;
}

std::shared_ptr<Entity> AttachmentComponent::getAttached(const std::string& code) const {
    for (const auto& attachment : attached) {
        if (attachment.code == code) {
            return attachment.entity;
        }
    }
    return nullptr;
}

AttachmentPoint& AttachmentComponent::attachEntity(const std::string& code, const std::shared_ptr<Entity>& entity) {
    //Create attachment point and set entity
    AttachmentPoint& attachment = attached.emplace_back();
    attachment.code = code;
    attachment.entity = entity;

    //Make it disabled and not selectable by default and set parent to us
    entity->setDisable(true);
    entity->setSelectable(false);
    entity->setParent(base);

    //Subscribe entity to simulation if wasn't added already
    Simulation* simulation = base->getSimulation();
    if (simulation && !entity->isActive()) {
        simulation->addEntity(entity);
    }
    base->changesCount++;

    return attachment;
}

void AttachmentComponent::detachEntity(const std::shared_ptr<Entity>& entity) {
    //Find and remove it
    bool found = false;
    for (auto it = attached.begin(); it != attached.end(); ++it) {
        if (it->entity == entity) {
            attached.erase(it);
            found = true;
            break;
        }
    }

    //If wasn't found then don't do anything
    if (!found) {
        return;
    }

    //Remove it from simulation and unset the parent
    Simulation* simulation = base->getSimulation();
    if (simulation && entity->isActive()) {
        simulation->removeEntity(entity);
    }
    entity->setParent(nullptr);
    base->changesCount++;
}

void AttachmentComponent::detachEntities() {
    const std::vector<AttachmentPoint> toRemove(attached);
    for (const auto& attachment : toRemove) {
        detachEntity(attachment.entity);
    }
}