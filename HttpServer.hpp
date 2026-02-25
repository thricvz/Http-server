#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "HttpData.hpp"

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

  std::string strip_string_from(const std::string& raw_string, char character ) ;
}




class HttpServer {
  public:
    enum class HttpServerStatus { ASLEEP, VALID, INVALID}; 

    static inline const int32_t MAX_CONNECTIONS = 3;  
    static inline const int32_t MAX_REQUEST_SIZE_BYTES = 3000;

    HttpServer(); 
    ~HttpServer() { close(m_socket); }; 
    
    void start();

  private:
    void launch_request_handler(const std::string& target,
        const std::string& body_data);

    void setup_server_sockets();

    HttpRequest    parse_request(const char* raw_data);
    HttpHeader     parse_header(const std::string& header) const; 

    // these should use iterators
    HttpOptions    parse_options() const;
    std::string    parse_body() const; 

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

void HttpServer::start() {
  if (m_status == HttpServerStatus::INVALID || m_status == HttpServerStatus::ASLEEP) 
    throw std::runtime_error("server must be in valid state to start\n");

  while ( true ) {
    int client_socket = accept(this->m_socket, NULL, NULL);

    if ( client_socket == -1) 
      continue;
     
    char http_request[MAX_REQUEST_SIZE_BYTES];
    memset(http_request, 0, MAX_REQUEST_SIZE_BYTES); 

    if ( recv(client_socket, http_request, MAX_REQUEST_SIZE_BYTES, 0) == -1 ) {
      perror("Failed to receive HTTP REQUEST : ");
      continue;
    }

    parse_request(http_request);
  };
}


void HttpServer::launch_request_handler(const std::string& target,
        const std::string& body_data) {};

HttpRequest HttpServer::parse_request(const char* raw_data) {
  HttpRequest http_request{};

  auto parsed_lines = parsing::split_string(raw_data, '\n');
  http_request.header = parse_header(parsed_lines[0]);

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
   header_fields[VERSION], 
  };
};

HttpOptions HttpServer::parse_options() const {


  return HttpOptions{};
};

std::string HttpServer::parse_body() const {
  // exctract header 
}; 

#endif
