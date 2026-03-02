#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "HttpData.hpp"
#include "HttpTargetHandler.hpp"

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

/* use templates to construct the actual urlmapping object
 *
 *
 * */

class UrlMap {
  public:
    UrlMap(const std::string& path) : m_path{path} {};

    virtual HttpTargetHandler* get() = 0;
    
    virtual bool is_mapped_to(const std::string& searched_path) const final{
      return searched_path == this->m_path;
    }

  private:
    std::string m_path{};
};

template<typename Handler>
struct custom_map : public UrlMap {
  custom_map(const char* path) : UrlMap(path) {};

  virtual HttpTargetHandler* get() {
    return &m_handler;
  };

  Handler m_handler {};
};

template<typename Handler>
UrlMap* map_to_url(const char* path) {
  static custom_map<Handler> request_handler(path);
  return &request_handler;
}


class HttpServer {
  public:
    enum class HttpServerStatus {ASLEEP, VALID, INVALID}; 

    static inline const int32_t MAX_CONNECTIONS = 3;  
    static inline const int32_t MAX_REQUEST_SIZE_BYTES = 3000;

    HttpServer(); 
    ~HttpServer() { close(m_socket); }; 
    
    void start() const;
    void register_url(UrlMap* mapping);

  private:

    void launch_request_handler(const HttpRequest& request, int32_t client_socket) const;
    void setup_server_sockets();

    HttpRequest     parse_request(const char* raw_data) const;
    HttpRequestLine parse_request_line(const std::string& request_line) const; 

    template<typename StartIterator, typename EndIterator>
    HttpOptions parse_headers(StartIterator start, EndIterator end) const; 

    template<typename StartIterator, typename EndIterator>
    std::string parse_body(StartIterator start, EndIterator end) const; 

    std::vector<UrlMap*> m_urls{};
    HttpServerStatus m_status {HttpServerStatus::ASLEEP};
    int              m_socket {-1};

};

#endif
