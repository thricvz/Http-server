#include <iostream>
#include "HttpTargetHandlers/HelloWorldHandler.hpp"
#include "Include/HttpServer.hpp"
#include <csignal>


int main() {
  
  HttpServer server(create_urls()
      .map_handler_to_url<HelloWorldHandler>("/hello")
      .map_handler_to_url<HelloWorldHandler>("/owaowa")
  );
    
  try {

    server.start();
  } catch(std::exception& error) {
    std::cerr << error.what();

  };
  return 0;
};
