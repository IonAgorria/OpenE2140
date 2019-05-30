//
// Created by Ion Agorria on 17/05/19
//
#ifndef OPENE2140_SIMULATION_PARAMETERS_H
#define OPENE2140_SIMULATION_PARAMETERS_H

#include "core/common.h"

/**
 * Basic simulation startup parameters
 */
struct SimulationParameters {
    /** Initial simulation seed for randomness */
    long seed = 0;
    /** World asset path to load */
    asset_path_t world = "";
    /** Players in this simulation */
    //TODO const std::vector<Player> players;
};
#endif //OPENE2140_SIMULATION_PARAMETERS_H
