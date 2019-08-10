//
// Created by Ion Agorria on 9/8/19
//
#include "animation.h"

bool Animation::isEnd() const {
    if (frames.empty()) {
        return true;
    }
    if (reverse) {
        return current == 0 && left <= 0;
    } else {
        return current >= frames.size() - 1 && left <= 0;
    }
}

void Animation::checkCurrent() {
    if (current >= frames.size()) {
        if (frames.empty()) {
            current = 0;
        } else {
            current = frames.size() - 1;
        }
    }
}

void Animation::setFrames(const std::vector<Image*>& images) {
    this->frames = images;
    checkCurrent();
}

size_t Animation::getCurrentIndex() const {
    return current;
}

void Animation::setCurrentIndex(size_t index) {
    this->current = index;
    checkCurrent();
    this->left = duration; //Reset left so the frame is not skipped
}

Image* Animation::getCurrentFrame() const {
    return frames.empty() ? nullptr : frames[current];
}

bool Animation::isFinished() const {
    return !loop && isEnd();
}

void Animation::update(duration_t delta) {
    if (isFinished()) {
        return;
    }
    //Decrement left
    left -= delta;
    //Loop until left is positive
    while (left <= 0) {
        left = 0;
        if (isFinished()) {
            break;
        }
        duration_t step = duration;
        if (reverse) {
            if (isEnd()) { //Need to change roll if we are in end
                current = frames.size();
                left += step;
            } else { //Current frame time over
                current--;
                left += step;
            }
        } else {
            if (isEnd()) { //Need to change roll if we are in end
                current = 0;
                left += step;
            } else { //Current frame time over
                current++;
                left += step;
            }
        }
        checkCurrent();
        if (step <= 0) {
            //Avoid infinite loop
            break;
        }
    }
}

std::string Animation::toString() const {
    return "Animation(" + toStringContent() + ")";
}

std::string Animation::toStringContent() const {
    return " Duration: " + std::to_string(duration) +
           " Left: " + std::to_string(left) +
           " Current: " + std::to_string(current) +
           " Loop: " + std::to_string(loop) +
           " Reverse: " + std::to_string(reverse)
    ;
}