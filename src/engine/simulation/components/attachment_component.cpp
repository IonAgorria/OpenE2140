//
// Created by Ion Agorria on 20/8/19
//

#include "engine/core/utils.h"
#include "engine/simulation/simulation.h"
#include "engine/simulation/entities/entity.h"
#include "engine/simulation/entities/entity_config.h"
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
                config_data_t entryValue = entry.value();
                if (!entryValue.is_object()) {
                    continue;
                }
                //TODO get code from obj and set position after creating entity
                entity_type_t type = {kind, 0, ""};
                base->getSimulation()->createEntity(type);
            }
        }
    } else {
        detachEntities();
    }
}

void AttachmentComponent::update() {
}

const std::vector<std::shared_ptr<Entity>>& AttachmentComponent::getAttached() const {
    return attached;
}

void AttachmentComponent::attachEntity(const std::shared_ptr<Entity>& entity) {
    Simulation* simulation = base->getSimulation();
    if (simulation) {
        simulation->addEntity(entity);
    }
    attached.emplace_back(entity);
    entity->setParent(base);
}

void AttachmentComponent::detachEntity(const std::shared_ptr<Entity>& entity) {
    Simulation* simulation = base->getSimulation();
    if (simulation) {
        simulation->removeEntity(entity);
    }
    Utils::eraseElementFromVector(attached, entity);
    entity->setParent(nullptr);
}

void AttachmentComponent::detachEntities() {
    const std::vector<std::shared_ptr<Entity>> toRemove(attached);
    for (const std::shared_ptr<Entity>& entity : attached) {
        detachEntity(entity);
    }
}