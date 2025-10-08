//
//  Timer.h
//  LightningCore
//
//  Created by Kian Marvi on 3/13/25.
//

#ifndef Timer_h
#define Timer_h
#include <mach/mach_time.h>
#include <iostream>

class Timer
{
public:
    Timer()
    {
        start = mach_absolute_time();
        mach_timebase_info(&timebase);
    }
    
    inline float GetElapsedTime()
    {
        uint64_t elapsed = mach_absolute_time() - start;
        return (elapsed * timebase.numer) / (timebase.denom * 1.0e9);
    }
    
private:
    uint64_t start;
    mach_timebase_info_data_t timebase;
};

#endif /* Timer_h */
