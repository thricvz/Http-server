#ifndef HELLO_WORLD_HANDLER_H
#define HELLO_WORLD_HANDLER_H

#include "HttpTargetHandler.hpp"
#include <iostream>

class HelloWorldHandler : public HttpTargetHandler {
  public: 
    void handle(const HttpRequest& request) const override {
      std::cout << "Hello i would like to tell this message: "  << request.body << std::endl;
    };
};

#endif
