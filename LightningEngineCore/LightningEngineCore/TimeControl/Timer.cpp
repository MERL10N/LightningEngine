//
//  Timer.cpp
//  LightningEngine
//
//  Created by Kian Marvi on 7/23/24.
//
#include <iostream>
#include <mach/mach_time.h>
#include "Timer.h"

namespace Timer
{
    float GetTimeInSeconds()
    {
        uint64_t time = mach_absolute_time();
        mach_timebase_info_data_t timebase;
        mach_timebase_info(&timebase);
        uint64_t elapsedNano = time * timebase.numer / timebase.denom;
        float elapsedSeconds = elapsedNano * 1e-9f;
        return elapsedSeconds;
    }
}
