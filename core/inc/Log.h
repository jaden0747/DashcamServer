#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Dashcam 
{

class Log
{
public:
    static void init();

    inline static std::shared_ptr<spdlog::logger>& getLogger() { return s_logger; }

private:
    static std::shared_ptr<spdlog::logger> s_logger;
};

// Client log macros
#define DASHCAM_TRACE(...)	      ::Dashcam::Log::getLogger()->trace(__VA_ARGS__)
#define DASHCAM_INFO(...)	      ::Dashcam::Log::getLogger()->info(__VA_ARGS__)
#define DASHCAM_WARN(...)	      ::Dashcam::Log::getLogger()->warn(__VA_ARGS__)
#define DASHCAM_ERROR(...)	      ::Dashcam::Log::getLogger()->error(__VA_ARGS__)
#define DASHCAM_FATAL(...)	      ::Dashcam::Log::getLogger()->critical(__VA_ARGS__)

}