//
// Created by Ion Agorria on 12/12/19
//
#ifndef OPENE2140_TREE_PRINTABLE_H
#define OPENE2140_TREE_PRINTABLE_H

#include <string>
#include "engine/io/log.h"
#include "engine/core/to_string.h"
#include "engine/core/types.h"

/**
 * Methods required for tree printing
 * Allows printing the structure of objects implementing this interface
 */
class ITreePrintable: public virtual IToString {
private:
    /**
     * Generic tree printer
     *
     * @param obj
     * @param prefix
     * @param leafs
     */
    void printTreeInternal(const log_ptr& log, const std::string& prefix = "",
                           size_t leafIndex = 0, size_t leafCount = 1) const {
        log->debug(prefix + (leafIndex == 0 ? "┌" : "├") +"── " + this->toString());
        size_t i = 0;
        auto leafs = this->getLeafs();
        for (const ITreePrintable* leaf : leafs) {
            leaf->printTreeInternal(log, prefix + "│ ", i, leafs.size());
            i++;
        }
        if ((leafIndex + 1) == leafCount) {
            log->debug(prefix + "┴");
        }
    }

public:
    /**
     * Destructor
     */
    ~ITreePrintable() override = default;

    /**
     * Return this element leafs
     */
    virtual std::vector<const ITreePrintable*> getLeafs() const {
        return std::vector<const ITreePrintable*>();
    }

    /**
     * Public callable tree printer
     *
     * @param log
     */
    virtual void printTree(const log_ptr& log) const {
        this->printTreeInternal(log);
    }
};

#endif //OPENE2140_TREE_PRINTABLE_H