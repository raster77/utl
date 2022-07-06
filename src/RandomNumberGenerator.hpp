#ifndef RANDOMNUNMBERGENERATOR_HPP_
#define RANDOMNUNMBERGENERATOR_HPP_

#include <cstdint>
#include <random>
#include <type_traits>

namespace utl {

  namespace abs {

    class ARandomNumberGenerator {

      public:
        ARandomNumberGenerator()
            : mGenerator(mRndDevice()) {
        }

        virtual ~ARandomNumberGenerator() {
        }

      protected:
        std::random_device mRndDevice;
        std::mt19937 mGenerator;
    };

  }

  template<typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
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
          return mDist(mGenerator);
        }

        void setDistribution(const T a, const T b) {
          setDistributionParams(a, b);
        }

      private:
        std::uniform_real_distribution<T> mDist;

        void setDistributionParams(const T a, const T b) {
          if (mDist.param().a() != a || mDist.param().b() != b) {
            mDist = std::uniform_real_distribution<T>(a, b);
          }
        }
    };

  template<typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
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
          return mDist(mGenerator);
        }

        void setDistribution(const T a, const T b) {
          setDistributionParams(a, b);
        }

      private:
        std::uniform_real_distribution<T> mDist;

        void setDistributionParams(const T a, const T b) {
          if (mDist.param().a() != a || mDist.param().b() != b) {
            mDist = std::uniform_real_distribution<T>(a, b);
          }
        }
    };

  template<typename T>
    class RealRng {

      public:

        static T number() {
          return realNumberGen.number();
        }

        static void setDistribution(const T a) {
          realNumberGen.setDistribution(-a, a);
        }

        static void setDistribution(const T a, const T b) {
          realNumberGen.setDistribution(a, b);
        }

      private:
        static RealNumberGenerator<T> realNumberGen;
    };

  template<typename T>
    RealNumberGenerator<T> RealRng<T>::realNumberGen;

  template<typename T>
    class IntegerRng {

      public:

        static T number() {
          return intNumberGen.number();
        }

        static void setDistribution(const T a) {
          intNumberGen.setDistribution(-a, a);
        }

        static void setDistribution(const T a, const T b) {
          intNumberGen.setDistribution(a, b);
        }

      private:
        static IntegerNumberGenerator<T> intNumberGen;
    };

  template<typename T>
  IntegerNumberGenerator<T> IntegerRng<T>::intNumberGen;

  using RngFloat = RealRng<float>;
  using RngDouble = RealRng<double>;
  using RngUInt8 = IntegerRng<uint8_t>;
  using RngInt8 = IntegerRng<int8_t>;
  using RngUInt16 = IntegerRng<uint16_t>;
  using RngInt16 = IntegerRng<int16_t>;
  using RngUInt32 = IntegerRng<uint32_t>;
  using RngInt32 = IntegerRng<int32_t>;
  using RngUInt64 = IntegerRng<uint64_t>;
  using RngInt64 = IntegerRng<int64_t>;

}

#endif /* RNG_RANDOMNUNMBERGENERATOR_HPP_ */

