#ifndef STR_HPP_
#define STR_HPP_

#include <algorithm>
#include <string>
#include <vector>

namespace utl {

  static inline std::string lTrim(const std::string& str) {
    std::string s(str);
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
      return !std::isspace(ch);
    }));
    return s;
  }

  // trim from end
  static inline std::string rTrim(const std::string& str) {
    std::string s(str);
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
      return !std::isspace(ch);
    }).base(), s.end());

    return s;
  }

  static inline std::string trim(const std::string& str) {
    return rTrim(lTrim(str));
  }

  static inline std::string toLower(const std::string& str) {
    std::string s(str);
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
      return tolower(c);
    });
    return s;
  }

  static inline std::string toUpper(const std::string& str) {
    std::string s(str);
    std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) {
      return toupper(c);
    });
    return s;
  }

  static inline std::string capitalize(const std::string& str) {
    std::string s(str);
    s[0] = std::toupper(s[0]);
    return s;
  }

  static inline bool equalsIgnoreCase(const std::string& str1, const std::string& str2) {
    std::string s1 = toLower(str1);
    std::string s2 = toLower(str2);
    return s1 == s2;
  }

  static inline void split(const std::string& str, const char& delimiter, std::vector<std::string>& results) {
    std::size_t index = 0;
    std::size_t i = 0;
    std::size_t startIndex = 0;
    std::size_t endIndex = 0;

    while (i <= str.length()) {
      if (str[i] == delimiter || i == str.length()) {
        endIndex = i;
        results.emplace_back(str.substr(startIndex, endIndex - startIndex));
        index++;
        startIndex = endIndex + 1;
      }
      i++;
    }
  }

}

#endif /* STR_HPP_ */
