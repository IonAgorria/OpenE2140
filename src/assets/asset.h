//
// Created by Ion Agorria on 8/04/18
//
#ifndef OPENE2140_ASSET_H
#define OPENE2140_ASSET_H

/**
 * Base asset containing any data for game
 */
class Asset {
private:
public:
    /**
     * Asset constructor
     */
    Asset() = default;

    /**
     * Asset destructor
     */
    virtual ~Asset() = default;

    /**
     * Disable copy constructor
     */
    Asset(const Asset& other) = delete;

    /**
     * Disable copy operator
     */
    void operator=(Asset const &other) = delete;
};

#endif //OPENE2140_ASSET_H
