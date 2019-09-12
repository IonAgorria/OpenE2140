//
// Created by Ion Agorria on 1/11/18
//

#include "engine/core/utils.h"
#include "engine/core/engine.h"
#include "engine/graphics/renderer.h"
#include "faction.h"
#include "player.h"
#include "entity.h"
#include "entity_store.h"
#include "components/player_component.h"
#include "src/engine/entities/entity_manager.h"
#include "world/world.h"
#include "engine/assets/asset.h"
#include "engine/assets/asset_level.h"
#include "simulation.h"

Simulation::Simulation(std::shared_ptr<Engine> engine, std::unique_ptr<SimulationParameters> parameters):
        parameters(std::move(parameters)), engine(std::move(engine)) {
    log = Log::get("Simulation");
    debugEntities = Utils::isFlag(FLAG_DEBUG_ALL);
    if (!this->parameters || this->parameters->world.empty()) {
        error = "Parameters not set";
        return;
    }
    //Load asset
    assetLevel = this->engine->getAssetManager()->getAsset<AssetLevel>(this->parameters->world);
    if (!assetLevel) {
        error = "World asset not found";
        return;
    }
}

void Simulation::loadWorld() {
    log->debug("Name: '" + assetLevel->name() + "'");

    //Load tileset
    std::unordered_map<unsigned int, Image*> tilesetImages;
    size_t tilesetSize = assetLevel->tilesetSize();
    for (size_t i = 0; i < tilesetSize; ++i) {
        asset_path_t path = assetLevel->tilePath(i);
        if (!path.empty()) {
            tilesetImages[i] = getImage(path);
        }
    }
    error = assetLevel->getError();
    if (hasError()) {
        return;
    }

    //Create world
    world = std::make_unique<World>(assetLevel, tilesetImages);
    error = world->getError();
    if (hasError()) {
        return;
    }
    tileSize = world->getTileSize();
    tileSizeHalf = tileSize / 2;
}

void Simulation::loadPlayers() {
    //Load players from parameters
    for (auto& parametersPlayer : parameters->players) {
        if (parametersPlayer) {
            addPlayer(std::move(parametersPlayer));
        }
    }

    //Load players from level (overwrites existing players or creates news)
    if (parameters->loadLevelContent) {
        std::vector<PlayerPrototype> levelPlayers;
        assetLevel->players(levelPlayers);
        error = assetLevel->getError();
        if (hasError()) {
            return;
        }
        for (PlayerPrototype& playerPrototype : levelPlayers) {
            Faction* faction = nullptr;
            if (playerPrototype.faction) {
                faction = getFaction(playerPrototype.faction);
            }

            Player* player = getPlayer(playerPrototype.id);
            if (!player) {
                std::unique_ptr playerPtr = std::make_unique<Player>(playerPrototype.id);
                player = playerPtr.get();
                addPlayer(std::move(playerPtr));
            }
            player->enemies = playerPrototype.enemies;
            player->money = playerPrototype.money;
            player->faction = faction;
        }
    }
}

void Simulation::loadEntities() {
    entityStore = std::make_unique<EntityStore>();

    //Load entities from level
    if (parameters->loadLevelContent) {
        std::vector<EntityPrototype> levelEntities;
        assetLevel->entities(levelEntities);
        error = assetLevel->getError();
        if (hasError()) {
            return;
        }
        for (EntityPrototype& entityPrototype : levelEntities) {
            if (!entityPrototype.exists) {
                //TODO these should be stored for later use
                continue;
            }
            createEntity(entityPrototype);
        }
    }
}

Simulation::~Simulation() {
    close();
}

void Simulation::close() {
    log->debug("Closing");
    if (entityStore) {
        std::vector<std::shared_ptr<Entity>> toRemove(entityStore->getEntities());
        for (std::shared_ptr<Entity>& entity : toRemove) {
            entity->removedFromSimulation();
        }
        entityStore->clear();
    }
    if (world) {
        world.reset();
    }
}

void Simulation::update() {
    world->update();

    std::vector<std::shared_ptr<Entity>> toRemove;
    for (const std::shared_ptr<Entity>& entity : entityStore->getEntities()) {
        //Parent already handles their entities
        if (entity->getParent()) {
            continue;
        }

        //Entity is destroyed
        if (entity->isDestroyed()) {
            toRemove.emplace_back(entity);
            continue;
        }

        //Update entity
        entity->update();
    }

    //Remove entities
    for (const std::shared_ptr<Entity>& entity : toRemove) {
        removeEntity(entity);
    }
}

void Simulation::draw(const Rectangle& rectangle, std::vector<std::shared_ptr<Entity>>& visibleEntities) {
    //Draw world
    Renderer* renderer = getRenderer();
    world->draw(renderer, rectangle);

    //Draw entities
    for (const std::shared_ptr<Entity>& entity : entityStore->getEntities()) {
        const Rectangle& bounds = entity->getBounds();
        if (rectangle.isOverlap(bounds)) {
            visibleEntities.emplace_back(entity);
            entity->draw();
            if (debugEntities) {
                renderer->drawRectangle(bounds, 2, Color::DEBUG_ENTITIES);
            }
        }
    }
}

EntityStore* Simulation::getEntitiesStore() const {
    return entityStore.get();
}

World* Simulation::getWorld() const {
    return world.get();
}

std::shared_ptr<Entity> Simulation::createEntity(const EntityPrototype& entityPrototype) {
    std::shared_ptr<Entity> entityPtr = engine->getEntityManager()->makeEntity(entityPrototype.type);
    if (entityPtr) {
        Entity* entity = entityPtr.get();
        //Basic stuff
        Vector2 position;
        toWorldVector(entityPrototype.position, position, true);
        entity->setPosition(position);
        entity->setDirection(entityPrototype.direction);
        entity->setDisable(entityPrototype.disabled);
        if (entityPrototype.player) {
            //Obtain player and component
            PlayerComponent* component = GET_COMPONENT(entity, PlayerComponent);
            Player* player = getPlayer(entityPrototype.player);
            if (player && component) {
                component->setPlayer(player);
            }
        }
        addEntity(entityPtr);
    }
    return entityPtr;
}

std::shared_ptr<Entity> Simulation::createEntity(
        const entity_type_t& entityType,
        bool addToSimulation) {
    std::shared_ptr<Entity> entity = engine->getEntityManager()->makeEntity(entityType);
    if (addToSimulation && entity) {
        addEntity(entity);
    }
    return entity;
}

std::shared_ptr<Entity> Simulation::createEntity(
        entity_kind_t entityKind, const std::string& entityCode,
        bool addToSimulation) {
    std::shared_ptr<Entity> entity = engine->getEntityManager()->makeEntity(entityKind, entityCode);
    if (addToSimulation && entity) {
        addEntity(entity);
    }
    return entity;
}

void Simulation::addEntity(const std::shared_ptr<Entity>& entity) {
    if (entity->isActive()) {
        BUG("Attempted to add already active entity {0} to simulation", entity->getID());
    }
    entity_id_t id = entityStore->add(entity);
    entity->addedToSimulation(id, this);
}

void Simulation::removeEntity(const std::shared_ptr<Entity>& entity) {
    if (!entity->isActive()) {
        BUG("Attempted to remove non active entity {0} from simulation", entity->getID());
    }
    entityStore->remove(entity);
    entity->removedFromSimulation();
}

Renderer* Simulation::getRenderer() const {
    return engine->getRenderer();
}

void Simulation::addFaction(std::unique_ptr<Faction> faction) {
    faction_id_t id = faction->id;
    //Allocate new empty if not enough are present
    if (factions.size() < static_cast<faction_id_t>(id + 1)) {
        factions.resize(id + 1);
    }
    factions[id].swap(faction);
}

std::vector<std::unique_ptr<Faction>>& Simulation::getFactions() {
    return factions;
}

Faction* Simulation::getFaction(faction_id_t id) const {
    return id < factions.size() ? factions[id].get() : nullptr;
}

Faction* Simulation::getFaction(const std::string& code) const {
    for (const std::unique_ptr<Faction>& faction : factions) {
        if (faction && faction->code == code) {
            return faction.get();
        }
    }
    return nullptr;
}

void Simulation::addPlayer(std::unique_ptr<Player> player) {
    player_id_t id = player->id;
    //Allocate new empty if not enough are present
    if (players.size() < static_cast<player_id_t>(id + 1)) {
        players.resize(id + 1);
    }
    players[id].swap(player);
}

std::vector<std::unique_ptr<Player>>& Simulation::getPlayers() {
    return players;
}

Player* Simulation::getPlayer(player_id_t id) const {
    return id < players.size() ? players[id].get() : nullptr;
}

/*
 * World helpers
 */

void Simulation::toTileVector(const Vector2& vector, Vector2& result) const {
    result.set(vector.x / tileSize, vector.y / tileSize);
}

void Simulation::toWorldVector(const Vector2& vector, Vector2& result, bool center) const {
    result.set(vector.x * tileSize, vector.y * tileSize);
    if (center) {
        result += tileSizeHalf;
    }
}

void Simulation::toTileRectangle(const Rectangle& rectangle, Rectangle& result) const {
    result.set(
            rectangle.x / tileSize, rectangle.y / tileSize,
            rectangle.w / tileSize, rectangle.h / tileSize
    );
}

void Simulation::toWorldRectangle(const Rectangle& rectangle, Rectangle& result, bool center) const {
    result.set(
            rectangle.x * tileSize, rectangle.y * tileSize,
            rectangle.w * tileSize, rectangle.h * tileSize
    );
    if (!center) {
        result.x -= tileSizeHalf;
        result.y -= tileSizeHalf;
    }
}

number_t Simulation::angleWorldVectors(const Vector2& origin, const Vector2& destination) const {
    return number_atan2(
            number_from_int(origin.x / tileSize), number_from_int(origin.y / tileSize),
            number_from_int(destination.x / tileSize), number_from_int(destination.y / tileSize)
    );
}

number_t Simulation::angleTileVectors(const Vector2& origin, const Vector2& destination) const {
    return number_atan2(
            number_from_int(origin.x), number_from_int(origin.y),
            number_from_int(destination.x), number_from_int(destination.y)
    );
}

/*
 * AssetManager proxy
 */

Image* Simulation::getImage(const asset_path_t& path) const {
    return engine->getAssetManager()->getImage(path).get();
}
