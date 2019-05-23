//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_LOG_H
#define OPENE2140_LOG_H

#include <list>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

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
     * Set's the level of a single logger with default value
     *
     * @param logger
     */
    static void set_default_level(log_ptr logger);

    /**
     * Closes all logs and sinks
     */
    static void closeAll();
};

#endif //OPENE2140_LOG_H
