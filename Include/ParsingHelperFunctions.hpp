#ifndef PARSING_HELPER_FUNCTIONS_H
#define PARSING_HELPER_FUNCTIONS_H

#include <cstdint>
#include <vector>
#include <string>

namespace parsing {
  static std::vector<std::string> split_string(const std::string& raw_string, char delimiter);

  static std::string strip_string_from(const std::string& raw_string, char character ); 
  
  static void replace(std::string& target, const std::string& to_replace, const std::string& replacement);
}


std::vector<std::string> parsing::split_string(const std::string &raw_string, char delimiter) {
    std::vector<std::string> split_strings{};  

    std::string search_string = raw_string;
    int32_t search_start{};
    auto split_position = search_string.find(delimiter, search_start);

    while (split_position != std::string::npos) {
      const int32_t substring_length = split_position;
      const int32_t rest_length = search_string.size() - split_position + 1;

      split_strings.push_back(search_string.substr(0, substring_length));
      search_string = search_string.substr(split_position+1, rest_length);
      split_position = search_string.find(delimiter, search_start);
    }
    
    split_strings.push_back(search_string);

    return split_strings;


}


std::string parsing::strip_string_from(const std::string& raw_string, char character ) {
  std::string copy_string = raw_string;
  auto delete_offset {copy_string.find(character)};

  while (delete_offset != std::string::npos) {
    copy_string.erase(copy_string.begin() + delete_offset);
    delete_offset = copy_string.find(character);
  }
  
  return copy_string;
};

void parsing::replace(std::string& target,
                      const std::string& to_replace,
                      const std::string& replacement) {
    
    const uint32_t to_replace_length = to_replace.size();
    auto found_index = target.find(replacement);

    while (found_index != std::string::npos) {
      target.replace(found_index, to_replace_length, replacement);
    }
}
#endif
