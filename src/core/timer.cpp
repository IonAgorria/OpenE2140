//
// Created by Ion Agorria on 03/04/18
//
#include "SDL_timer.h"
#include "timer.h"

Timer::Timer() {
    frequency = SDL_GetPerformanceFrequency();
    counter = SDL_GetPerformanceCounter();
}

void Timer::update() {
    counter = SDL_GetPerformanceCounter();
}

float Timer::elapsed() {
    return (float) ((double) (SDL_GetPerformanceCounter() - counter) / frequency);
}

void Timer::wait(float time) {
    time -= elapsed();
    if (0 < time) {
        SDL_Delay((Uint32) (time * 1000));
    }
}
