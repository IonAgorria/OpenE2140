//
// Created by Ion Agorria on 21/04/19
//
#ifndef OPENE2140_ASSET_PROCESSOR_DATPAL_H
#define OPENE2140_ASSET_PROCESSOR_DATPAL_H

#include "assets/iassetprocessor.h"

class AssetProcessorDatPal: public IAssetProcessor {
private:
    virtual void processIntermediates() override;
};

#endif //OPENE2140_ASSET_PROCESSOR_DATPAL_H
