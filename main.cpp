#include <iostream>
#include "HttpServer.hpp"
#include <csignal>


int main() {
  HttpServer server;
  try {

    server.start();
  } catch(...) {};
  return 0;
};
