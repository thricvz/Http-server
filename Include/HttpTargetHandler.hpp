#ifndef HTTP_TARGET_HANDLER_H
#define HTTP_TARGET_HANDLER_H

#include "HttpData.hpp"
#include <cstdint>
#include <memory>

class HttpTargetHandler {
  public: 
    virtual void handle(const HttpRequest& request, int32_t connection_socket) const = 0;
};

#endif
