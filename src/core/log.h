//
// Created by Ion Agorria on 22/03/18
//
#ifndef OPENE2140_LOG_H
#define OPENE2140_LOG_H

#include <list>
#include <memory>
#include "spdlog/spdlog.h"

using log_ptr = std::shared_ptr<spdlog::logger>;

class Log {
private:
    static std::list<spdlog::sink_ptr> sinks;
public:
    static log_ptr get(const std::string& name);
    static void closeAll();
};

#endif //OPENE2140_LOG_H
