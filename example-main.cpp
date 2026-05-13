#include <iostream>
#include <csignal>

#include "Include/HttpServer.hpp"
#include "HttpRouters/HelloWorldHandler.hpp"
#include "HttpRouters/UserHandler.hpp"

int main() {
  RessourceManager* const ressources = RessourceManager::Instance();
  
  HttpServer server(create_urls()
      .map_handler_to_url<UserRouter>("/", ressources)
      .map_handler_to_url<HelloWorldRouter>("/user", ressources)
  );
    
  try {

    server.start();

  } catch(std::exception& error) {
    std::cerr << error.what();
  };
  
  return 0;
};
