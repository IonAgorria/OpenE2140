//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_LOG_H
#define OPENE2140_LOG_H

#include <list>
#include <memory>
#include "spdlog/spdlog.h"

/** Log type */
using log_ptr = std::shared_ptr<spdlog::logger>;

/**
 * Log class for static methods
 */
class Log {
private:
    /**
     * Sinks to use when creating logs
     */
    static std::list<spdlog::sink_ptr> sinks;
public:
    /**
     * Creates or return's existing log pointer
     *
     * @param name of log
     * @return log
     */
    static log_ptr get(const std::string& name);

    /**
     * Closes all logs and sinks
     */
    static void closeAll();
};

#endif //OPENE2140_LOG_H
