//
// Created by Ion Agorria on 20/8/19
//

#include "engine/io/config.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/entity.h"
#include "engine/entities/entity_config.h"
#include "attachment_component.h"

void AttachmentComponent::construction() {
}

AttachmentComponent::~AttachmentComponent() {
    //Cleanup lefotovers
    if (!attached.empty()) {
        Log::get()->warn("Attached entities when entity {0} was destroyed", base->getID());
        detachEntities();
    }
}

void AttachmentComponent::setup() {
}

void AttachmentComponent::simulationChanged() {
    const EntityConfig* config = base->getConfig();
    if (base->isActive()) {
        config_data_t attachments = config->getData("attachments");
        if (attachments.is_object()) {
            entity_kind_t kind = config->getData("attachments_kind", 0);
            for (auto entry = attachments.begin(); entry != attachments.end(); ++entry) {
                //Read the attachment entry and get the type
                config_data_t entryValue = entry.value();
                if (!entryValue.is_object()) continue;
                std::string code = entryValue["code"].get<std::string>();

                //Create the entity and attach it
                std::shared_ptr<Entity> entity = base->getSimulation()->createEntity(kind, code);
                AttachmentPoint& attachment = attachEntity(entity);

                //Set the position
                Config::getVector2(entryValue["position"], attachment.position);
            }
        }
    } else {
        detachEntities();
    }
}

void AttachmentComponent::update() {
    //Update the position of each entity and propagate update
    for (const auto& attachment : attached) {
        Entity* entity = attachment.entity.get();
        Vector2 position = base->getPosition();
        position += attachment.position;
        entity->setPosition(position);
        entity->update();
    }
}

const std::vector<AttachmentPoint>& AttachmentComponent::getAttached() const {
    return attached;
}

AttachmentPoint& AttachmentComponent::attachEntity(const std::shared_ptr<Entity>& entity) {
    //Create attachment point and set entity
    AttachmentPoint& attachment = attached.emplace_back();
    attachment.entity = entity;

    //Subscribe entity to simulation and set parent to us
    Simulation* simulation = base->getSimulation();
    if (simulation && !entity->isActive()) {
        simulation->addEntity(entity);
    }
    entity->setParent(base);

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
}

void AttachmentComponent::detachEntities() {
    const std::vector<AttachmentPoint> toRemove(attached);
    for (const auto& attachment : toRemove) {
        detachEntity(attachment.entity);
    }
}