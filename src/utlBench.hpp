#ifndef UTLBENCH_HPP
#define UTLBENCH_HPP

#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>
#include <cpuid.h>
#include <cstring>

#ifndef clock
#if defined _WIN32 || _WIN64
#define clock std::chrono::steady_clock
#else
#define clock std::chrono::high_resolution_clock
#endif
#endif

namespace utl
{
namespace sb
{
enum TimeUnit {
    Nano = 1,
    Micro = 1000,
    Milli = 1000000,
    Second = 1000000000
};

class Bench
{
public:
    Bench(const std::size_t iterations = 100)
        : mIterations(iterations)
        , maxStrLength(0)
        , mTimeUnit(Nano)
    {
    }

    ~Bench()
    {
    }

    void add(const std::string& testName, void func(void))
    {
        if(testName.length() > maxStrLength)
            maxStrLength = testName.length();
        mFunctions.emplace_back(testName, func);
    }

    void run(const bool warmUp = true)
    {
        std::string tuStr = " " + timeUnitToStr(mTimeUnit);
        std::size_t precision = 3;
        std::to_string(mTimeUnit).length() > 3 ? precision = std::to_string(mTimeUnit).length() : precision;

        results.clear();
        if(warmUp) {
            exec(false);
        }
        exec(true);

        std::cout << "Running on " << getCpu() << std::endl;
        std::cout << "Iterations: " << mIterations << std::endl << std::endl;
        std::cout  << std::setw(10) << "Benchmark\t" << std::setw(10) << "Min\t" << std::setw(10) << "Max\t" << std::setw(10) << "Avg" << std::setw(0) << std::endl;
        for(auto& r : results) {
            std::cout<< std::setw(10) << r.name << "\t"<< std::fixed
                     << std::setw(10) << r.min / static_cast<double>(mTimeUnit) << tuStr << "\t"
                     << std::setw(10) << r.max / static_cast<double>(mTimeUnit) << tuStr << "\t"
                     << std::setw(10) << r.avg / static_cast<double>(mTimeUnit) << tuStr
                     << std::endl;
        }
    }

    void clear()
    {
        results.clear();
        mFunctions.clear();
        maxStrLength = 0;
    }

    const std::size_t getIterations() const
    {
        return mIterations;
    }

    void setIterations(const std::size_t iterations)
    {
        mIterations = iterations;
    }

    const TimeUnit getTimeUnit() const
    {
        return mTimeUnit;
    }

    void setTimeUnit(const TimeUnit timeUnit)
    {
        mTimeUnit = timeUnit;
    }

private:
    struct Result {
        std::string name;
        double min;
        double max;
        double avg;
        Result(const std::string& name, const double min, const double max, const double avg)
            : name(name)
            , min(min)
            ,max(max)
            , avg(avg)
        {}
    };
    std::size_t mIterations;
    std::vector<std::pair<std::string, void (*)()>> mFunctions;
    std::vector<Result> results;
    std::size_t maxStrLength;
    TimeUnit mTimeUnit;

    void exec(const bool log)
    {
        for(auto& f : mFunctions) {
            double time = 0;
            double min = 1000000.d;
            double max = 0;
            for(std::size_t i = 0; i < mIterations; ++i) {
                auto start = clock::now();
                f.second();
                double res = std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - start).count();
                time += res;
                min = std::min(min, res);
                max = std::max(max, res);
            }
            if(log) {
                results.emplace_back(Result(f.first, min, max, time / static_cast<double>(mIterations)));
            }
        }
    }

    std::string timeUnitToStr(const TimeUnit timeUnit)
    {
        switch(timeUnit) {
        case Nano:
            return "ns";
        case Micro:
            return "µs";
        case Milli:
            return "ms";
        default:
            return "s";
        }
    }

    std::string getCpu()
    {
        char CPUBrandString[0x40];
        unsigned int CPUInfo[4] = {0,0,0,0};

        __cpuid(0x80000000, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);
        unsigned int nExIds = CPUInfo[0];

        memset(CPUBrandString, 0, sizeof(CPUBrandString));

        for (unsigned int i = 0x80000000; i <= nExIds; ++i) {
            __cpuid(i, CPUInfo[0], CPUInfo[1], CPUInfo[2], CPUInfo[3]);

            if (i == 0x80000002)
                memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
            else if (i == 0x80000003)
                memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
            else if (i == 0x80000004)
                memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
        }
        return trim(static_cast<std::string>(CPUBrandString));
    }

    std::string trim(const std::string& str)
    {
        std::size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first) {
            return str;
        }
        std::size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
    }
};
}
}
#endif // UTLBENCH_HPP
