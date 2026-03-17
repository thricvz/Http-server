#ifndef USER_ROUTER_H
#define USER_ROUTER_H


#include "../Include/HttpRouter.hpp"
#include "../Include/HttpResponse.hpp"

#include <cstdint>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include "../Include/json.hpp"

#include <memory>
using json = nlohmann::json;

class UserRouter : public HttpRouter {
  public: 
    void handle(const HttpRequest& request, int32_t connection_socket) const override {

      HttpResponse response;
      json response_message = 
      {
        {"developper", "eric"},
        {"age", 20},
        {"nationality", "cape-verdean"}
      };

      response.add_status_line({"HTTP/1.1", "OK", 200})
        .add_header("Access-Control-Allow-Origin", "*")
        .add_body(std::move(response_message));

      if (send(connection_socket, response.content.c_str(), response.content.size(), 0) <= 0) {
        std::cerr << "[ Error "<< request.request_line.target << " ] : failed to send response \n";
      };
      close(connection_socket);
    };


};

#endif
