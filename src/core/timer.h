//
// Created by Ion Agorria on 03/04/18
//
#ifndef OPENE2140_TIMER_H
#define OPENE2140_TIMER_H

#include "SDL_types.h"

/**
 * Implements timer object using SDL2 timers as a backend
 */
class Timer {
private:
    /**
     * SDL2 timer frequency divisor
     */
    Uint64 frequency; //SDL2 type
    /**
     * SDL2 timer counter
     */
    Uint64 counter; //SDL2 type
public:
    /**
     * Constructs timer
     */
    Timer();

    /**
     * Updates timer state
     */
    void update();

    /**
     * Calculates how much time elapsed since last update
     *
     * @return elapsed seconds
     */
    float elapsed();

    /**
     * Waits specified time from last update
     *
     * @param time to wait
     */
    void wait(float time);
};

#endif //OPENE2140_TIMER_H
