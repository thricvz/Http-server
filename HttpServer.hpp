#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <exception>
#include <errno.h>

struct HttpHeaderData {


};

struct HttpOptions {


};


class HttpServer {
  public:
    static int32_t inline MAX_CONNECTIONS = 3;  
  // constructor
    HttpServer(); 
    
    void start();

  private:
    void parse_request(const std::string& target,
        const std::string& body_data);

    HttpHeaderData parse_header(const char * raw_data) const; 
    HttpOptions    parse_options(const char * raw_data) const;
    std::string    parse_body(const char * raw_data) const; 

    int m_socket {-1};
};


void HttpServer::start() {
  if (this->m_socket == -1) 
    throw std::runtime_error("Cannot run listen on invalid socket");

  listen(this->m_socket, MAX_CONNECTIONS);
  while ( true ) {

  };
}

HttpServer::HttpServer() {
  // setup socket   
  this->m_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->m_socket == -1) {
    std::cerr << "Failed to create socket \n";
  }

  sockaddr_in local_address;
  local_address.sin_family = AF_INET;
  local_address.sin_port = htonl(80);
  inet_aton("127.0.0.1", &local_address.sin_addr); 

  if (bind(this->m_socket, (sockaddr*) &local_address, sizeof(local_address)) == - 1) {
    std::cerr << "failed to bind\n";
  }

}

void HttpServer::parse_request(const std::string& target,
        const std::string& body_data) {


};

HttpHeaderData HttpServer::parse_header(const char * raw_data) const {

  return HttpHeaderData{};
};

HttpOptions HttpServer::parse_options(const char * raw_data) const {


  return HttpOptions{};
};

std::string HttpServer::parse_body(const char * raw_data) const {
  
  return {};
}; 

#endif
