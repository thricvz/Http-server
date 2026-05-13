#ifndef HTTP_ROUTER_H
#define HTTP_ROUTER_H

#include "HttpData.hpp"
#include "RessourceManager.hpp"

#include <cstdint>

class HttpRouter {
  public: 
    HttpRouter(RessourceManager *ressources) : m_ressources(ressources){};

    virtual void handle(const HttpRequest& request, int32_t connection_socket) const = 0;

  private:
    RessourceManager* m_ressources{};
};

#endif
