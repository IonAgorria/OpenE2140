//
// Created by Ion Agorria on 25/03/2017
//
#ifndef OPENE2140_ANIMATION_H
#define OPENE2140_ANIMATION_H

#include <vector>
#include "engine/core/types.h"
#include "engine/core/macros.h"
#include "engine/core/to_string.h"

class Image;

/**
 * Reproduces the animation of several images
 */
class Animation: public IToString {
protected:
    /** Frames of animation */
    std::vector<Image*> frames;
    /** Current frame */
    size_t current = 0;
    /** Time to pass in current frame */
    duration_t left = 0;

    /**
     * @return if animation reached end.
     */
    bool isEnd() const;

    /**
     * Ensures current index doesn't go out of bounds
     */
    void checkCurrent();

public:
    /** Time to pass in each frame */
    duration_t duration = 0;
    /** Repeat the loop or stay at end when finish */
    bool loop = false;
    /** Sets animation to backwards */
    bool reverse = false;

    /**
     * Constructor
     */
    Animation() = default;

    /**
     * Destructor
     */
    ~Animation() override = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Animation)

    /**
     * Sets the animation frames
     */
    void setFrames(const std::vector<Image*>& images);

    /** return current frame index */
    size_t getCurrentIndex() const;

    /**
     * Sets the frame index.
     *
     * @param index frame to set as current
     */
    void setCurrentIndex(size_t index);

    /** @return the current frame. */
    Image* getCurrentFrame() const;

    /** @return if it's finished in case of non loop animation. */
    bool isFinished() const;

    /**
     * Updates animation
     *
     * @param delta The time in ms since the last update.
     */
    void update(duration_t delta);

    /*
     * IToString
     */

    std::string toString() const override;

    std::string toStringContent() const override;
};

#endif //OPENE2140_ANIMATION_H
