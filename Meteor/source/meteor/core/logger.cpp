#include "pch.h"
#include "logger.h"

#include <spdlog\sinks\stdout_color_sinks.h>

namespace meteor
{
    std::shared_ptr<spdlog::logger> Logger::ms_EngineLogger;
    std::shared_ptr<spdlog::logger> Logger::ms_AppLogger;

    // --------------------------------------------------------------------------------------------------------------------------------------
    Logger::Logger()
    {
    }

    // --------------------------------------------------------------------------------------------------------------------------------------
    void Logger::Initialize()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        ms_EngineLogger  = spdlog::stdout_color_mt("ENGINE");
        ms_AppLogger   = spdlog::stdout_color_mt("APP");

        ms_EngineLogger->set_level(spdlog::level::trace);
        ms_AppLogger->set_level(spdlog::level::trace);
    }
}