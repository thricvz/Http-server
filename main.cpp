#include <iostream>
#include "HttpTargetHandlers/HelloWorldHandler.hpp"
#include "Include/HttpServer.hpp"
#include <csignal>


int main() {
  HttpServer server;
  server.register_url(map_to_url<HelloWorldHandler>("/hello"));
  try {

    server.start();
  } catch(std::exception& error) {
    std::cerr << error.what();

  };
  return 0;
};
