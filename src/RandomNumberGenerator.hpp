#ifndef RANDOMNUNMBERGENERATOR_HPP_
#define RANDOMNUNMBERGENERATOR_HPP_

#include <cstdint>
#include <random>

namespace utl {

  namespace abs {

    class ARandomNumberGenerator {

      public:
        ARandomNumberGenerator()
          : mGenerator(mRndDevice()) {
        }

        ~ARandomNumberGenerator() {
        }

      protected:
        std::random_device mRndDevice;
        std::mt19937 mGenerator;
    };

  }

  template<typename T>
    class RealNumberGenerator : public abs::ARandomNumberGenerator {

      public:

        RealNumberGenerator()
          : ARandomNumberGenerator()
          , mDist(0, 1) {
        }

        ~RealNumberGenerator() {
        }

        T interval(const T min, const T max) {
          setDistributionParams(min, max);
          return mDist(mGenerator);
        }

        T range(const T min, const T max) {
          return min + number() * (max - min);
        }

        T range(const T width) {
          setDistributionParams(-width, width);
          return mDist(mGenerator);
        }

        T number() {
          setDistributionParams(0, 1);
          return mDist(mGenerator);
        }

      private:
        std::uniform_real_distribution<T> mDist;

        void setDistributionParams(const T a, const T b) {
          if (mDist.param().a() != 0 && mDist.param().b() != 1) {
            mDist = std::uniform_real_distribution<T>(a, b);
          }
        }
    };

  template<typename T>
    class IntegerNumberGenerator : public abs::ARandomNumberGenerator {

      public:

      IntegerNumberGenerator()
        : ARandomNumberGenerator()
        , mDist(0, 1) {
        }

        ~IntegerNumberGenerator() {
        }

        T interval(const T min, const T max) {
          setDistributionParams(min, max);
          return mDist(mGenerator);
        }

        T range(const T min, const T max) {
          return min + number() * (max - min);
        }

        T range(const T width) {
          setDistributionParams(-width, width);
          return mDist(mGenerator);
        }

        T number() {
          setDistributionParams(0, 1);
          return mDist(mGenerator);
        }

      private:
        std::uniform_real_distribution<T> mDist;

        void setDistributionParams(const T a, const T b) {
          if (mDist.param().a() != a || mDist.param().b() != b) {
            mDist = std::uniform_real_distribution<T>(a, b);
          }
        }
    };

  typedef RealNumberGenerator<float> FloatNumberGenerator;
  typedef RealNumberGenerator<double> DoubleNumberGenerator;

  typedef IntegerNumberGenerator<int64_t> Int64NumberGenerator;
  typedef IntegerNumberGenerator<int32_t> Int32NumberGenerator;
  typedef IntegerNumberGenerator<int16_t> Int16NumberGenerator;
  typedef IntegerNumberGenerator<int8_t> Int8NumberGenerator;
  typedef IntegerNumberGenerator<uint64_t> UInt64NumberGenerator;
  typedef IntegerNumberGenerator<uint32_t> UInt32NumberGenerator;
  typedef IntegerNumberGenerator<uint16_t> UInt16NumberGenerator;
  typedef IntegerNumberGenerator<uint8_t> UInt8NumberGenerator;

}

#endif /* RNG_RANDOMNUNMBERGENERATOR_HPP_ */


