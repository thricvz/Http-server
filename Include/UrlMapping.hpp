#ifndef URL_MAPPING_H
#define URL_MAPPING_H

#include "RessourceManager.hpp"
#include "HttpRouter.hpp"
#include <stdexcept>


class UrlMapper{
  friend UrlMapper& create_urls();

  public:
    template<typename TargetHandler> 
    UrlMapper& map_handler_to_url(const std::string& target, RessourceManager* ressources) {
      static TargetHandler new_handler(ressources);       
      m_url_map[target]  = &new_handler;

      return *this;
    };

    HttpRouter * retrieve_handler(const std::string& target) const {
      try {
        auto& handler = m_url_map.at(target);
        return handler;

      } catch(std::out_of_range& error) {
        return nullptr;
      }
    }

   operator UrlMapper* () {
      return this;
   };

  private:
    UrlMapper() = default;

    std::map<std::string, HttpRouter*> m_url_map{};
}; 


inline UrlMapper& create_urls() {
  static UrlMapper single_instance;
  return single_instance; 
}
#endif
