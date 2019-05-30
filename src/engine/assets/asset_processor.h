//
// Created by Ion Agorria on 21/04/19
//
#ifndef OPENE2140_IASSETPROCESSOR_H
#define OPENE2140_IASSETPROCESSOR_H

#include "engine/core/error_possible.h"

class AssetManager;
/**
 * Processor for asset manager loading process
 */
class IAssetProcessor: public IErrorPossible {
protected:
    /**
     * Asset manager which this processor belongs
     */
    AssetManager* manager;

public:
    /**
     * Destructor
     */
    virtual ~IAssetProcessor() = default;

    /**
     * Assigns the current manager for processor
     */
    void setManager(AssetManager* current);

    /**
     * Scans a container which contains assets to get the contents (folder or mapped file with paths)
     *
     * @param path of containers root
     * @param name of container
     * @return true if succeeded
     */
    virtual bool scanContainer(const std::string& path, const std::string& name);

    /**
     * Scans assets from directory container and stores in manager
     *
     * @param path to use as assets root
     * @param name of container
     * @return amount of loaded assets or -1 if error occurred
     */
    virtual int scanContainerDir(const std::string& path, const std::string& name);

    /**
     * Processes intermediate assets if they can be converted to different assets
     */
    virtual void processIntermediates();
};

#endif //OPENE2140_IASSETPROCESSOR_H
