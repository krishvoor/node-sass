#include "util_string.hpp"

#include <iostream>
#include <algorithm>

namespace Sass {
  namespace Util {

    // ##########################################################################
    // Special case insensitive string matcher. We can optimize
    // the more general compare case quite a bit by requiring
    // consumers to obey some rules (lowercase and no space).
    // - `literal` must only contain lower case ascii characters
    // there is one edge case where this could give false positives
    // test could contain a (non-ascii) char exactly 32 below literal
    // ##########################################################################
    bool equalsLiteral(const char* lit, const std::string& test) {
      // Work directly on characters
      const char* src = test.c_str();
      // There is a small chance that the search string
      // Is longer than the rest of the string to look at
      while (*lit && (*src == *lit || *src + 32 == *lit)) {
        ++src, ++lit;
      }
      // True if literal is at end
      // If not test was too long
      return *lit == 0;
    }
    // EO equalsLiteral

    // ###########################################################################
    // Returns [name] without a vendor prefix.
    // If [name] has no vendor prefix, it's returned as-is.
    // ###########################################################################
    std::string unvendor(const std::string& name)
    {
      if (name.size() < 2) return name;
      if (name[0] != '-') return name;
      if (name[1] == '-') return name;
      for (size_t i = 2; i < name.size(); i++) {
        if (name[i] == '-') return name.substr(i + 1);
      }
      return name;
    }
    // EO unvendor

    std::string rtrim(const std::string& str) {

      std::string trimmed = str;
      size_t pos_ws = trimmed.find_last_not_of(" \t\n\v\f\r");
      if (pos_ws != std::string::npos) {
        trimmed.erase(pos_ws + 1);
      }
      else {
        trimmed.clear();
      }
      return trimmed;
    }

    std::string normalize_newlines(const std::string& str) {
      std::string result;
      result.reserve(str.size());
      std::size_t pos = 0;
      while (true) {
        const std::size_t newline = str.find_first_of("\n\f\r", pos);
        if (newline == std::string::npos) break;
        result.append(str, pos, newline - pos);
        result += '\n';
        if (str[newline] == '\r' && str[newline + 1] == '\n') {
          pos = newline + 2;
        }
        else {
          pos = newline + 1;
        }
      }
      result.append(str, pos, std::string::npos);
      return result;
    }

    std::string normalize_underscores(const std::string& str) {
      std::string normalized = str;
      std::replace(normalized.begin(), normalized.end(), '_', '-');
      return normalized;
    }

    std::string normalize_decimals(const std::string& str) {
      std::string normalized;
      if (!str.empty() && str[0] == '.') {
        normalized.reserve(str.size() + 1);
        normalized += '0';
        normalized += str;
      }
      else {
        normalized = str;
      }
      return normalized;
    }

    char opening_bracket_for(char closing_bracket) {
      switch (closing_bracket) {
      case ')': return '(';
      case ']': return '[';
      case '}': return '{';
      default: return '\0';
      }
    }

    char closing_bracket_for(char opening_bracket) {
      switch (opening_bracket) {
      case '(': return ')';
      case '[': return ']';
      case '{': return '}';
      default: return '\0';
      }
    }

  }
  // namespace Util

}
// namespace Sass