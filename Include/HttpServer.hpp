#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "HttpData.hpp"
#include "HttpRouter.hpp"
#include "UrlMapping.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
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


class UrlMap;

class HttpServer {
  public:

    enum class HttpServerStatus {ASLEEP, VALID, INVALID}; 

    static inline const int32_t MAX_CONNECTIONS = 3;  
    static inline const int32_t MAX_REQUEST_SIZE_BYTES = 3000;

    HttpServer(UrlMapper* url_map); 
    ~HttpServer() { close(m_socket); }; 
    
    virtual void start() const final;
    
  protected:
    virtual void send_error(int32_t connection_socket, const std::string& error_description, uint32_t error_code) const ;

  private:

    void launch_request_handler(const HttpRequest& request, int32_t client_socket) const;
    void setup_server_sockets();


    HttpRequest     parse_request(const char* raw_data) const;
    HttpRequestLine parse_request_line(const std::string& request_line) const; 

    template<typename StartIterator, typename EndIterator>
    HttpOptions parse_headers(StartIterator start, EndIterator end) const; 

    template<typename StartIterator, typename EndIterator>
    std::string parse_body(StartIterator start, EndIterator end) const; 

    UrlMapper* m_urls{};
    HttpServerStatus m_status {HttpServerStatus::ASLEEP};
    int              m_socket {};

};

#endif
