//
// Created by Ion Agorria on 5/08/19
//

#include "entity_factory.h"
#include "entity_config.h"

void EntityConfig::loadData(const config_data_t& configData, const IEntityFactory* factory) {
    IHasConfigData::loadData(configData);
    code = getData<const std::string>("code", "");
    type = getData<const std::string>("type", "");
    loadSprites(factory);
}

void EntityConfig::loadSprites(const IEntityFactory* factory) {
    log_ptr log = factory->getLog();
    const std::string defaultPath = configData.value<const std::string>("sprites_path", "");
    if (defaultPath.empty()) {
        log->error("{0} sprites_path is empty", toString());
        return;
    }
    config_data_t spritesData = configData["sprites"];
    if (spritesData.is_object()) {
        for (auto entry = spritesData.begin(); entry != spritesData.end(); ++entry) {
            config_data_t value = entry.value();
            if (value.is_number_unsigned()) {
                //If its just a number then use it as index for a single image
                unsigned int index = value.get<unsigned int>();
                std::vector<Image*> images;
                const asset_path_t imagePath = defaultPath + std::to_string(index);
                Image* image = factory->getImage(imagePath);
                if (image) {
                    images.emplace_back(image);
                } else {
                    log->error("{0} sprites {1} image not found at {2}", toString(), entry.key(), imagePath);
                }
                sprites[entry.key()] = images;
            } else if (value.is_array()) {
                //If its a array then use it as a collection of indexes
                std::vector<Image*> images;
                for (nlohmann::json& element : value) {
                    if (!element.is_number_unsigned()) {
                        log->error("{0} sprites {1} non unsigned number found in array", toString(), entry.key());
                        continue;
                    }
                    unsigned int index = element.get<unsigned int>();
                    const asset_path_t imagePath = defaultPath + std::to_string(index);
                    Image* image = factory->getImage(imagePath);
                    if (image) {
                        images.emplace_back(image);
                    } else {
                        log->error("{0} sprites {1} image not found at {2}", toString(), entry.key(), imagePath);
                    }
                }
                sprites[entry.key()] = images;
            } else if (value.is_object()) {
                //If its a object then treat it as a set of rules to assemble collections
                //Get base index
                if (!value["index"].is_number_unsigned()) {
                    log->error("{0} sprites {1} no index found in object", toString(), entry.key());
                    continue;
                }
                unsigned int index = value["index"].get<unsigned int>();
                //Number of collection in this sprite group
                unsigned int collections = value["collections"].is_number_unsigned()
                                         ? value["collections"].get<unsigned int>() : 1;
                //Get assets path or use the default one
                std::string assetPath = value["sprite_path"].is_string()
                                      ? value["sprite_path"].get<std::string>() : defaultPath;
                //Amount of images in each image set
                unsigned int length = value["length"].is_number_unsigned()
                                    ? value["length"].get<unsigned int>() : 1;
                //Number to skip per each collection
                unsigned int separation = value["separation"].is_number_unsigned()
                                        ? value["separation"].get<unsigned int>() : 0;

                //Iterate each collection (set of images)
                for (unsigned int ci = 0; ci < collections; ++ci) {
                    //Get the current index as start
                    unsigned int start = index;
                    //Advance the index by length of collection
                    index += length;
                    //Create the collection
                    std::vector<Image*> images;
                    for (unsigned int i = start; i < index; ++i) {
                        const asset_path_t imagePath = assetPath + std::to_string(i);
                        Image* image = factory->getImage(imagePath);
                        if (image) {
                            images.emplace_back(image);
                        } else {
                            log->error("{0} sprites {1} image not found at {2}", toString(), entry.key(), imagePath);
                        }
                    }
                    //Set the name and store collection
                    std::string group_name = entry.key();
                    if (1 < collections) {
                        group_name += "_" + std::to_string(ci);
                    }
                    sprites[group_name] = images;
                    //Advance by separation
                    index += separation;
                }
            } else {
                log->error("{0} sprites {1} unknown sprite data", toString(), entry.key());
            }
        }
    } else if (!spritesData.is_null()) {
        log->error("{0} sprites root is not object nor null", toString());
    }
}

std::string EntityConfig::toString() const {
    return "EntityConfig(" + toStringContent() + ")";
}

std::string EntityConfig::toStringContent() const {
    return "Kind: " + std::to_string(kind) + ":" + std::to_string(id) +
          (code.empty() ? "" : " " + code) +
          (type.empty() ? "" : " " + type) +
          "";
}
