//
// Created by Ion Agorria on 13/06/19
//
#ifndef OPENE2140_PATH_HANDLER_H
#define OPENE2140_PATH_HANDLER_H

#include <unordered_map>
#include "engine/core/macros.h"
#include "path_request.h"

class Tile;
class Player;

/**
 * Handles per player pathfinder requests and coordinates the ongoing requests by the agents
 *
 * Considerations
 *
 * The following requests can arise due to nature of a RTS game with multiple fixed and moving agents in grid:
 * - One agent -> one fixed destination
 * - One agent -> one moving destination/agent
 * - Multiple agents close to each other (group) -> one fixed destination
 * - Multiple agents close to each other (group) -> one moving destination/agent
 * - Multiple agents or groups not close to each other -> one fixed destination
 * - Multiple agents or groups not close to each other -> one moving destination/agent
 *
 * With the following changing events during the path solving and navigation to destination:
 * - The previously blocked path is now navigable (a blocking agent was destroyed or moved)
 * - New short term obstacle appears in path (such as moving agent crossing paths)
 * - New long term obstacle appears in path (such as building or a idle agent)
 * - Destination is no longer reachable
 * - Destination is now reachable
 *
 * The following are the requirements:
 * - Each request must be able to give the following states during or as result: None, Computing, Partial, Fail, Success
 * - Keeping the pathfinder internal state separate from the shared game state (only reading is allowed)
 * - Keeping the pathfinder state separate for each player, as one player state must not interfere other players states
 */
class PathHandler {
protected:
    /**
     * Pointer which belongs to
     */
    Player* player = nullptr;

    /**
     * Ongoing requests
     */
    std::vector<std::shared_ptr<PathRequest>> requests;

public:
    /**
     * Constructs a new path handler for player
     * @param player
     */
    explicit PathHandler(Player* player);

    /**
     * Destructor
     */
    PathHandler() = default;

    /**
     * @return player
     */
    Player* getPlayer();

    /**
     * Removes entity from any ongoing requests
     *
     * @param entity_id
     */
    void removeRequests(entity_id_t entity_id);

    /**
     * Returns a request for entity with the provided destination
     *
     * @param entity the entity originating the request
     * @param tile destination tile to find the path
     * @param partial is request for a partial type?
     */
    std::shared_ptr<PathRequest> requestDestination(entity_id_t entity_id, Tile* tile, bool partial = false);

    /**
     * Returns a request for entity with the provided target
     *
     * @param entity the entity originating the request
     * @param target entity to set the destination tile
     */
    std::shared_ptr<PathRequest> requestTarget(entity_id_t entity_id, const std::shared_ptr<Entity>& target);

    /**
     * Updates the ongoing requests
     */
    void update();
};

#endif //OPENE2140_PATH_HANDLER_H
