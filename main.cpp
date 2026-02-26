#include <iostream>
#include "HttpServer.hpp"
#include <csignal>


int main() {
  HttpServer server;
  try {

    server.start();
  } catch(std::exception& error) {
    std::cerr << error.what();

  };
  return 0;
};
