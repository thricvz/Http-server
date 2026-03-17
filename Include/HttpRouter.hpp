#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include "HttpData.hpp"
#include <cstdint>
#include <memory>

class HttpRouter {
  public: 
    virtual void handle(const HttpRequest& request, int32_t connection_socket) const = 0;
};

#endif
