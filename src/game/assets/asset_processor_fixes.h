//
// Created by Ion Agorria on 19/08/19
//
#ifndef OPENE2140_ASSET_PROCESSOR_FIXES_H
#define OPENE2140_ASSET_PROCESSOR_FIXES_H

#include "engine/assets/asset_processor.h"

/**
 * Processes the assets to apply corrections
 */
class AssetProcessorFixes: public IAssetProcessor {
private:
    void refreshAssets() override;
};

#endif //OPENE2140_ASSET_PROCESSOR_FIXES_H
