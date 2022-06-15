#ifndef UTLBENCH_HPP
#define UTLBENCH_HPP

#include <algorithm>
#include <chrono>
#include <cpuid.h>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace utl {

  enum TimeUnit {
    Nano = 1, Micro = 1000, Milli = 1000000, Second = 1000000000
  };

  class Bench {
    public:

      Bench(const std::size_t iterations = 100)
        : mIterations(iterations)
        , maxStrLength(0)
        , mTimeUnit(Nano)
        , mPrecision(2) {
      }

      ~Bench() {
      }

      void add(const std::string& testName, void func(void)) {
        maxStrLength = std::max(maxStrLength, testName.length());
        mFunctions.emplace_back(testName, func);
      }

      void run(const bool warmUp = true) {
        std::string tuStr = " " + timeUnitToStr(mTimeUnit);
        std::size_t precision = 3;
        std::to_string(mTimeUnit).length() > 3 ?
            precision = std::to_string(mTimeUnit).length() : precision;

        results.clear();
        if (warmUp) {
          exec(false);
        }
        exec(true);

        std::cout << "Running on " << getCpu() << std::endl;
        std::cout << "Iterations: " << mIterations << std::endl << std::endl;
        std::cout << std::setw(maxStrLength) << "Benchmark\t" << std::setw(10)
            << "Min\t" << std::setw(10) << "Max\t" << std::setw(10) << "Avg"
            << std::setw(0) << std::endl;
        for (auto &r : results) {
          std::cout << std::setw(maxStrLength) << std::left << r.name << "\t"
              << std::fixed << std::setw(10) << std::internal
              << std::setprecision(mPrecision)
              << r.min / static_cast<double>(mTimeUnit) << tuStr << "\t"
              << std::setw(10) << std::setprecision(mPrecision)
              << r.max / static_cast<double>(mTimeUnit) << tuStr << "\t"
              << std::setw(10) << std::setprecision(mPrecision)
              << r.avg / static_cast<double>(mTimeUnit) << tuStr << std::endl;
        }
      }

      void save(const std::string& filename, const std::string& title = "") {
        std::ofstream file;
        file.open(filename);

        file << "{";
        file << "\"title\": \"" << title << "\",";
        file << "\"tu\": \"" << timeUnitToStr(mTimeUnit) << "\",";
        file << "\"data\": [";
        std::size_t idx = 0;
        for (auto &r : results) {
          file << "[\"" << r.name << "\", " << std::fixed
              << std::setprecision(mPrecision)
              << r.avg / static_cast<double>(mTimeUnit) << "]";

          if (idx < results.size() - 1) {
            file << ",";
          }

          idx++;
        }
        file << "]}";

        file.close();
      }

      void sort() {
        std::sort(results.begin(), results.end());
      }

      void clear() {
        results.clear();
        mFunctions.clear();
        maxStrLength = 0;
      }

      const std::size_t getIterations() const {
        return mIterations;
      }

      void setIterations(const std::size_t iterations) {
        mIterations = iterations;
      }

      const std::size_t getPrecision() const {
        return mPrecision;
      }

      void setPrecision(const std::size_t precision) {
        mPrecision = precision;
      }

      const TimeUnit getTimeUnit() const {
        return mTimeUnit;
      }

      void setTimeUnit(const TimeUnit timeUnit) {
        mTimeUnit = timeUnit;
      }

    private:
      struct Result {
          std::string name;
          double min;
          double max;
          double avg;
          Result(const std::string& name, const double min, const double max, const double avg)
              : name(name), min(min), max(max), avg(avg) {
          }

          bool operator<(const Result& r) {
            return avg < r.avg;
          }
      };
      std::size_t mIterations;
      std::vector<std::pair<std::string, void (*)()>> mFunctions;
      std::vector<Result> results;
      std::size_t maxStrLength;
      TimeUnit mTimeUnit;
      std::size_t mPrecision;

      void exec(const bool log) {
        for (auto &f : mFunctions) {
          double time = 0;
          double min = 1000000.;
          double max = 0;
          for (std::size_t i = 0; i < mIterations; ++i) {
            auto start = std::chrono::steady_clock::now();
            f.second();
            double res = std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::steady_clock::now() - start).count();
            time += res;
            min = std::min(min, res);
            max = std::max(max, res);
          }
          if (log) {
            results.emplace_back(
                Result(f.first, min, max,
                       time / static_cast<double>(mIterations)));
          }
        }
      }

      std::string timeUnitToStr(const TimeUnit timeUnit) {
        switch (timeUnit) {
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

      std::string getCpu() {
        char CPUBrandString[0x40];
        unsigned int CPUInfo[4] = { 0, 0, 0, 0 };

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

      std::string trim(const std::string& str) {
        std::size_t first = str.find_first_not_of(' ');
        if (std::string::npos == first) {
          return str;
        }
        std::size_t last = str.find_last_not_of(' ');
        return str.substr(first, (last - first + 1));
      }
  };

}
#endif // UTLBENCH_HPP
