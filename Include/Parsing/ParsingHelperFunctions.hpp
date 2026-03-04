#ifndef PARSING_HELPER_FUNCTIONS_H
#define PARSING_HELPER_FUNCTIONS_H

#include <cstdint>
#include <vector>
#include <string>

namespace parsing {
  std::vector<std::string> split_string(const std::string& raw_string, char delimiter);

  std::string strip_string_from(const std::string& raw_string, char character ); 
  
  void replace(std::string& target, const std::string& to_replace, const std::string& replacement);
}


#endif
