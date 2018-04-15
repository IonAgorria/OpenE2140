//
// Created by Ion Agorria on 22/03/18
//
#include "log.h"
#include "utils.h"
#include "config.h"

std::list<spdlog::sink_ptr> Log::sinks;

log_ptr Log::get(const std::string& name) {
    std::string logName = Utils::padRight(name, 10);

    //Check if exists
    std::shared_ptr<spdlog::logger> logger = spdlog::get(logName);
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
            sinks.push_back(std::make_shared<spdlog::sinks::simple_file_sink_mt>(logPath, true));
        }
    }

    //Create logger
    logger = std::make_shared<spdlog::logger>(logName, begin(Log::sinks), end(Log::sinks));
    logger->set_level(Utils::isDebug() ? spdlog::level::debug : spdlog::level::info);
    logger->flush_on(spdlog::level::warn);
    logger->set_pattern("[%H:%M:%S.%e][%L][%n] %v");
    spdlog::register_logger(logger);
    return logger;
}

void Log::closeAll() {
    //Drop the logs
    spdlog::drop_all();

    //Clear sinks
    if (!Log::sinks.empty()) {
        Log::sinks.clear();
    }
}