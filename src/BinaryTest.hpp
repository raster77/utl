#ifndef BINARYTEST_HPP
#define BINARYTEST_HPP

#include <functional>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

/*
test.add("test", true, std::function<bool()>(
    []() {
        float a = 1.f;
        float b = 1.f;
        return a - b == 0.f;
    })
);
*/

namespace utl {

  class BinaryTest {
    public:
      BinaryTest(const std::string& title = "Test", const bool enhancedDisplay = false)
        : maxSize(0)
        , mTitle(title)
        , mEnhancedDisplay(enhancedDisplay) {
      }

      ~BinaryTest() {
      }

      void add(const std::string& name, const bool expected, std::function<bool()> func) {
        maxSize = std::max(name.size(), maxSize);
        tests.emplace_back(name, expected, func);
      }

      void setTitle(const std::string& title) {
        mTitle = title;
      }

      void run() {
        std::size_t passed = 0;
        std::size_t failed = 0;
        const std::string_view ok = mEnhancedDisplay ? "\x1b[1;32m\u2713" : "";
        const std::string_view ko = mEnhancedDisplay ? "\x1b[1;31m\u2717" : "";
        const std::string_view passedStr = mEnhancedDisplay ? "\x1b[1;32mpassed\n" : "passed\n";
        const std::string_view failedStr = mEnhancedDisplay ? "\x1b[1;31mfailed\n" : "failed\n";

        std::cout << mTitle << std::endl << std::endl;

        for (auto &t : tests) {

          const bool b = t.expectedResult == t.func();
          b ? passed++ : failed++;
          std::cout << (b ? ok : ko)
              << (mEnhancedDisplay ? "\x1b[0;37m" : "")
              << std::setw(maxSize + 2)
              << std::left
              << t.name << ": "
              << std::setw(6)
              << std::right
              << (t.expectedResult == t.func() ? passedStr : failedStr);
        }
        std::cout << std::endl
            << std::right
            << (mEnhancedDisplay ? "\x1b[0;37m" : "")
            << "Total tests : "
            << tests.size()
            << std::endl;
        std::cout << std::setw(15)
            << std::left
            << "  Passed : "
            << passed << std::endl;
        std::cout << std::setw(15)
            << std::left << "  Failed : "
            << failed << std::endl;

        if (mEnhancedDisplay) {
          std::cout << "\x1b[0m" << std::endl;
        }
      }

      void clear() {
        tests.clear();
        maxSize = 0;
      }

    private:
      struct Test {
          std::string name;
          bool expectedResult;
          std::function<bool()> func;

          Test(const std::string& name, bool expectedResult, std::function<bool()> func)
            : name(name)
            , expectedResult(expectedResult)
            , func(func) {
          }
      };

      std::vector<Test> tests;
      std::size_t maxSize;
      std::string mTitle;
      bool mEnhancedDisplay;
  };

}

#endif // BINARYTEST_HPP
