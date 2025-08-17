//
//  Log.h
//  LightningCore
//
//  Created by Kian Marvi on 8/5/25.
//

#ifndef Log_h
#define Log_h

#include <memory>
#include <spdlog/spdlog.h>

class Log
{
public:
    static void Init();
    
    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
    
private:
    static std::shared_ptr<spdlog::logger>  s_CoreLogger;
    static std::shared_ptr<spdlog::logger>  s_ClientLogger;
};

#endif /* Log_h */
