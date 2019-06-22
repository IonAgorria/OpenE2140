//
// Created by Ion Agorria on 22/03/18
//
#include "log.h"
#include "engine/core/utils.h"
#include "engine/core/common.h"

std::list<spdlog::sink_ptr> Log::sinks;

log_ptr Log::get(const std::string& name) {
    std::string logName = Utils::padRight(name, 10);

    //Check if exists
    log_ptr logger = spdlog::get(logName);
    if (logger) {
        return logger;
    }

    //Fill sinks if empty
    if (sinks.empty()) {
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
        //Get user path for log sink if available
        std::string logPath = Utils::getUserPath();
        if (!logPath.empty()) {
            logPath = logPath + GAME_LOG_FILE;
            sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath, true));
        }
    }

    //Create logger
    logger = std::make_shared<spdlog::logger>(logName, begin(Log::sinks), end(Log::sinks));
    Log::set_default_level(logger);
    logger->flush_on(spdlog::level::info);
    logger->set_pattern("[%H:%M:%S.%e][%L][%n] %v");
    spdlog::register_logger(logger);
    return logger;
}

log_ptr Log::get() {
    return  Log::get(MAIN_LOG);
}

void Log::set_default_level(log_ptr logger) {
    logger->set_level(Utils::isDebug() ? spdlog::level::debug : spdlog::level::info);
}

void Log::closeAll() {
    //Drop the logs
    spdlog::drop_all();

    //Clear sinks
    if (!Log::sinks.empty()) {
        Log::sinks.clear();
    }
}