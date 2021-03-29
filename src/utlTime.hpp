#ifndef UTLTIME_HPP
#define UTLTIME_HPP

#include <chrono>
#ifndef clock
#if defined _WIN32 || _WIN64
    #define clock std::chrono::steady_clock
#else
    #define clock std::chrono::high_resolution_clock
#endif
#endif

namespace utl
{
    namespace time
    {
        
        std::chrono::time_point<clock> getTimePoint()
        {
            return clock::now();
        }

        template <typename Duration = std::milli>
        double elapsedTime(const std::chrono::time_point<clock>& start,
            const std::chrono::time_point<clock>& end)
        {
            return std::chrono::duration<double, Duration>(end - start).count();
        }
    }
}

#endif // UTLRANDOM_HPP
