//
// Created by Ion Agorria on 16/06/19
//
#ifndef OPENE2140_PATH_STATE_H
#define OPENE2140_PATH_STATE_H

/**
 * Path states enum
 */
enum class PathFinderStatus {
    /** Idle/Unknown state */
    None,
    /** Path is being computed */
    Computing,
    /** Closest approximation path without reaching destination */
    Partial,
    /** No available path */
    Fail,
    /** Path reaches destination */
    Success
};

#endif //OPENE2140_PATH_STATE_H
