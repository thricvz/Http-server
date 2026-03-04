#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE


#include "Parsing/ParsingHelperFunctions.hpp"
#include <cstdint>
#include <string>

struct HttpStatusLine{
  std::string protocol_version;
  std::string reason_phrase;
  uint32_t code {};
};
// refactor so that it doesnt matter in which order we add the status_line headers or body
struct HttpResponse {
HttpResponse() = default;
~HttpResponse() = default;

HttpResponse(const HttpResponse& other) {
  this->content = other.content;
};

HttpResponse& operator=(const HttpResponse& other) {
  this->content = other.content;
  return *this;
} 

HttpResponse(HttpResponse&&) = delete;
void operator=(HttpResponse&&) = delete;


HttpResponse& add_status_line(HttpStatusLine _status_line) {
  std::string status_line = _status_line.protocol_version     + " "
                          + std::to_string(_status_line.code) + " " 
                          + _status_line.reason_phrase        + "\r\n"; 

  content.insert(0, std::move(status_line)); 
  return *this;
}

HttpResponse& add_header(const std::string& name, const std::string& value) {
    std::string header_line = name + ": " + value + "\r\n";
    content.append(header_line);
    return *this;
  }

  HttpResponse& add_body(std::string&& body_content) {
      std::string body{body_content};
      parsing::replace(body, "\n", "\r\n");
      content.append("\r\n");
      content.append(body);

    return *this; 
  }


  std::string content; 
  private: 
    int32_t n_headers{};
};

#endif
