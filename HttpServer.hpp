#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "HttpData.hpp"
#include "HttpTargetHandler.hpp"
#include "HelloWorldHandler.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <string>

#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <exception>
#include <errno.h>



namespace parsing {
  std::vector<std::string> split_string(const std::string& raw_string, char delimiter) {
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

  std::string strip_string_from(const std::string& raw_string, char character ) {
    std::string copy_string = raw_string;
    auto delete_offset {copy_string.find(character)};

    while (delete_offset != std::string::npos) {
      copy_string.erase(copy_string.begin() + delete_offset);
      delete_offset = copy_string.find(character);
    }
    
    return copy_string;
  } ;
}




class HttpServer {
  public:

    enum class HttpServerStatus { ASLEEP, VALID, INVALID}; 

    static inline const int32_t MAX_CONNECTIONS = 3;  
    static inline const int32_t MAX_REQUEST_SIZE_BYTES = 3000;

    HttpServer(); 
    ~HttpServer() { close(m_socket); }; 
    
    void start() const;

    // some way to registervoid register(const std::string& path, HttpTargetHandler& handler);

  private:
    void launch_request_handler(const HttpRequest& request) const;

    void setup_server_sockets();

    HttpRequest  parse_request(const char* raw_data) const;
    HttpHeader   parse_header(const std::string& header) const; 

    template<typename StartIterator, typename EndIterator>
    HttpOptions  parse_options(StartIterator start, EndIterator end) const; 

    template<typename StartIterator, typename EndIterator>
    std::string  parse_body(StartIterator start, EndIterator end) const; 


  
    HttpServerStatus m_status {HttpServerStatus::ASLEEP};
    int m_socket {-1};
};

HttpServer::HttpServer() {
  setup_server_sockets();
}

void HttpServer::setup_server_sockets() {
  m_status = HttpServerStatus::INVALID;

  this->m_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->m_socket == -1) {
    perror("Failed to create socket \n");
    return;
  }

  sockaddr_in local_address;
  local_address.sin_family = AF_INET;
  local_address.sin_port = htons(8080);
  local_address.sin_addr.s_addr = inet_addr("127.0.0.1");  

  if (bind(this->m_socket, (sockaddr*) &local_address, sizeof(local_address)) == - 1) {
    perror("failed to bind");
    close(this->m_socket);
    return;
  }

  if (listen(this->m_socket, MAX_CONNECTIONS) == -1) {
    perror("failed to listen on socket");
    close(this->m_socket);
    return;
  };
  
  m_status = HttpServerStatus::VALID;
}

void HttpServer::start() const {
  if (m_status == HttpServerStatus::INVALID || m_status == HttpServerStatus::ASLEEP) 
    throw std::runtime_error("server must be in valid state to start\n");

  while ( true ) {
    int client_socket = accept(this->m_socket, NULL, NULL);

    if ( client_socket == -1) 
      continue;
     
    char raw_http_request[MAX_REQUEST_SIZE_BYTES];
    memset(raw_http_request, 0, MAX_REQUEST_SIZE_BYTES); 

    if ( recv(client_socket, raw_http_request, MAX_REQUEST_SIZE_BYTES, 0) == -1 ) {
      perror("Failed to receive HTTP REQUEST : ");
      continue;
    }
    
    auto http_request = parse_request(raw_http_request);
    launch_request_handler(http_request); 
  };
}


void HttpServer::launch_request_handler(const HttpRequest& request) const {
  // consult the mapping that is done with register
  if (request.header.target == "/hello") {
    const HttpTargetHandler &handler = HelloWorldHandler();
    handler.handle(request);
  }
};

HttpRequest HttpServer::parse_request(const char* raw_data) const {
  const std::string OPTIONS_BODY_SEPARATOR{"\r"};
  HttpRequest http_request{};

  auto parsed_lines = parsing::split_string(raw_data, '\n');
  http_request.header = parse_header(parsed_lines[0]);

  const auto& options_start = parsed_lines.begin() + 1;
  const auto& options_end = std::find(parsed_lines.begin()+1 , parsed_lines.end(), OPTIONS_BODY_SEPARATOR);
 
  const auto& body_start = options_end + 1;
  const auto& body_end = parsed_lines.end();

  http_request.options = parse_options(options_start, options_end);
  http_request.body = parse_body(body_start, body_end);

  return http_request;
};

HttpHeader HttpServer::parse_header(const std::string& header) const {
  const int32_t METHOD{0};
  const int32_t REQUEST_TARGET{1};
  const int32_t VERSION{2};

  auto header_fields = parsing::split_string(header, ' ');

  return HttpHeader{
   get_http_method(header_fields[METHOD]), 
   header_fields[REQUEST_TARGET], 
   parsing::strip_string_from(header_fields[VERSION], '\r'), 
  };
};

template<typename StartIterator, typename EndIterator>
HttpOptions  HttpServer::parse_options(StartIterator data_start, EndIterator data_end) const {
  auto extract_field_and_value = [](const std::string& line) {
    const int32_t split_index = line.find(": ") ;
    const int32_t end = line.size() - split_index;
    const int32_t separator_length = 2;

    return std::pair<std::string, std::string> {
      line.substr(0,split_index),
      line.substr(split_index + separator_length, end)
    };
  };

  HttpOptions options{};
  
  for ( ;data_start != data_end; data_start++)  {
    std::string line = parsing::strip_string_from(*data_start, '\r');
    const auto field_value_tuple = extract_field_and_value(line);
    options.fields[field_value_tuple.first] = field_value_tuple.second;
  }

  return options;
};

template<typename StartIterator, typename EndIterator>
std::string  HttpServer::parse_body(StartIterator start_data, EndIterator end_data) const {
  std::string body_data;
  for (; start_data != end_data ; start_data++ ) {
    body_data += *start_data;
  }
  return body_data;
}; 

#endif
