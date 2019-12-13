//
// Created by Ion Agorria on 23/07/19
//
#ifndef OPENE2140_LOCALE_H
#define OPENE2140_LOCALE_H

#include <unordered_map>
#include "engine/core/macros.h"
#include "engine/core/error_possible.h"
#include "engine/core/to_string.h"

using locale_texts_t = std::unordered_map<std::string, std::string>;

/**
 * Provides translations for game texts in a single language
 */
class Locale: public IErrorPossible, public IToString {
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
     * Visible name of this locale
     */
    const std::string name;

    /**
     * Constructor
     */
    Locale(const std::string& code, const std::string& name, const std::string& path);

    /**
     * Destructor
     */
    virtual ~Locale() = default;

    /**
     * Macros
     */
    NON_COPYABLE_NOR_MOVABLE(Locale)
    TYPE_NAME_OVERRIDE(Locale)

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

    /*
     * IToString
     */

    std::string toStringContent() const override;
};

#endif //OPENE2140_LOCALE_H
