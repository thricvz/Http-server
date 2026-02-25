#ifndef HTTP_DATA_H
#define HTTP_DATA_H

#include <string>

enum class HttpMethod {
  GET,
  POST,
  DELETE,
  UNKNOWN
};

HttpMethod get_http_method(const std::string& method) {
  if (method == "GET")    return HttpMethod::GET;
  if (method == "POST")   return HttpMethod::POST;
  if (method == "DELETE") return HttpMethod::DELETE;

  return HttpMethod::UNKNOWN;
}



struct HttpHeader {
  HttpMethod method;
  std::string target; 
  std::string version;
};

struct HttpOptions {


};


struct HttpRequest {
  HttpHeader  header{};
  HttpOptions options{};
  std::string body{};
};


#endif
