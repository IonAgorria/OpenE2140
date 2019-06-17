//
// Created by Ion Agorria on 16/06/19
//
#ifndef OPENE2140_PATH_STATE_H
#define OPENE2140_PATH_STATE_H

/**
 * Path states enum
 */
enum class PathFinderStatus {
    None,
    Computing,
    Partial,
    Fail,
    Success
};

#endif //OPENE2140_PATH_STATE_H
