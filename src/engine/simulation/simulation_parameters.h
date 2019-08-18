//
// Created by Ion Agorria on 17/05/19
//
#ifndef OPENE2140_SIMULATION_PARAMETERS_H
#define OPENE2140_SIMULATION_PARAMETERS_H

#include "engine/core/common.h"
#include "engine/simulation/player.h"

/**
 * Basic simulation startup parameters
 */
struct SimulationParameters {
    /** Initial simulation seed for randomness */
    long seed = 0;
    /** World asset path to load */
    asset_path_t world = "";
    /** Load level players and entities */
    bool loadLevelContent = false;
    /** Players in this simulation */
    std::vector<std::unique_ptr<Player>> players;
};
#endif //OPENE2140_SIMULATION_PARAMETERS_H
