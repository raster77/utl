#ifndef UTLCHRONO_HPP
#define UTLCHRONO_HPP

#include <chrono>

namespace utl {

  class Chrono {

    public:

      Chrono(const bool autoStart = false)
        : mStart{}
        , mDuration(0)
        , mRunning(false) {
        if (autoStart) {
          start();
        }
      }

      ~Chrono() {
      }

      /**
       * @brief Start the timer
       *
       */
      void start() {
        mDuration = 0.;
        mStart = std::chrono::steady_clock::now();
        mRunning = true;
      }

      /**
       * @brief Stop the timer
       *
       */
      void stop() {
        mDuration += getDuration();
        mRunning = false;
      }

      /**
       * @brief Resume the timer
       *
       */
      void resume() {
        mStart = std::chrono::steady_clock::now();
        mRunning = true;
      }

      /**
       * @brief Elapsed time as nano seconds
       *
       * @return elapsed time as nano seconds
       */
      const double asNanoSeconds() {
        if (mRunning) {
          return mDuration + getDuration();
        }
        return mDuration;
      }

      /**
       * @brief Elapsed time as micro seconds
       *
       * @return elapsed time as micro seconds
       */
      const double asMicroSeconds() {
        if (mRunning) {
          return (mDuration + getDuration()) / TO_MICRO;
        }
        return mDuration > 0. ? mDuration / TO_MICRO : 0.;
      }

      /**
       * @brief Elapsed time as milli seconds
       *
       * @return elapsed time as milli seconds
       */
      const double asMilliSeconds() {
        if (mRunning) {
          return (mDuration + getDuration()) / TO_MILLI;
        }
        return mDuration > 0. ? mDuration / TO_MILLI : 0.;
      }

      /**
       * @brief Elapsed time as seconds
       *
       * @return elapsed time as seconds
       */
      const double asSeconds() {
        if (mRunning) {
          return (mDuration + getDuration()) / TO_SEC;
        }
        return mDuration > 0. ? mDuration / TO_SEC : 0.;
      }

    private:
      static constexpr double TO_MICRO = 1000.0;
      static constexpr double TO_MILLI = TO_MICRO * 1000.0;
      static constexpr double TO_SEC = TO_MILLI * 1000.0;
      std::chrono::time_point<std::chrono::steady_clock> mStart;
      double mDuration;
      bool mRunning;

      double getDuration() {
        const std::chrono::duration<double, std::nano> isZero = mStart
            - std::chrono::time_point<std::chrono::steady_clock> {};

        if (isZero.count() > 0) {
          return (std::chrono::steady_clock::now() - mStart).count();
        }

        return 0;
      }

  };

}
#endif // UTLCHRONO_HPP
