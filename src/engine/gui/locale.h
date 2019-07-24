//
// Created by Ion Agorria on 23/07/19
//
#ifndef OPENE2140_LOCALE_H
#define OPENE2140_LOCALE_H

#include "engine/core/common.h"
#include "engine/core/error_possible.h"

using locale_texts_t = std::unordered_map<std::string, std::string>;

/**
 * Provides translations for game texts in a single language
 */
class Locale: public IErrorPossible {
protected:
    /**
     * Internal text map
     */
    locale_texts_t texts;

public:
    /**
     * Code of this locale
     */
    const std::string code;

    /**
     * Constructor
     */
    Locale(const std::string& code, const std::string& path);

    /**
     * Destructor
     */
    virtual ~Locale() = default;

    /**
     * Disable copy/move
     */
    NON_COPYABLE_NOR_MOVABLE(Locale)

    /**
     * Loads the texts from config data
     *
     * @param path config containing texts
     */
    virtual void load(const std::string& path);

    /**
     * Provides this locale text version of text key
     *
     * @param key of text
     * @return localized text
     */
    const std::string& getText(const std::string& key);
};

#endif //OPENE2140_LOCALE_H
