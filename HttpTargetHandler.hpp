#ifndef HTTP_TARGET_HANDLER_H
#define HTTP_TARGET_HANDLER_H

#include "HttpData.hpp"

class HttpTargetHandler {
  public: 
    virtual void handle(const HttpRequest& request) const = 0;
};

#endif
