#ifndef HTTP_DATA_H
#define HTTP_DATA_H

#include <string>
#include <map>

enum class HttpRequestMethod {
  GET,
  POST,
  DELETE,
  UNKNOWN
};

static HttpRequestMethod  get_http_method(const std::string& method) {
  if (method == "GET")    return HttpRequestMethod::GET;
  if (method == "POST")   return HttpRequestMethod::POST;
  if (method == "DELETE") return HttpRequestMethod::DELETE;

  return HttpRequestMethod::UNKNOWN;
}



struct HttpRequestLine {
  HttpRequestMethod method;
  std::string target; 
  std::string version;
};

struct HttpOptions {
  std::map<std::string,std::string> fields{};
};


struct HttpRequest {
  HttpRequestLine  request_line{};
  HttpOptions headers{};
  std::string body{};
};


#endif
