//
//  Log.cpp
//  LightningCore
//
//  Created by Kian Marvi on 8/5/25.
//

#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

 std::shared_ptr<spdlog::logger>  Log::s_CoreLogger;
 std::shared_ptr<spdlog::logger>  Log::s_ClientLogger;

void Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    
    s_CoreLogger = spdlog::stdout_color_mt("Lightning Editor");
    s_CoreLogger->set_level(spdlog::level::trace);
    
    s_ClientLogger = spdlog::stdout_color_mt("Lightning Game");
    s_ClientLogger->set_level(spdlog::level::trace);
}
