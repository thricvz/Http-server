#include "Include/HttpServer.hpp"

#include "Include/HttpResponse.hpp"
#include "Include/HttpRouter.hpp"
#include "Include/Parsing/ParsingHelperFunctions.hpp"

#include "HttpRouters/HelloWorldHandler.hpp"
#include "Include/UrlMapping.hpp"
#include <cstdint>

HttpServer::HttpServer(UrlMapper* mapper) : m_urls(mapper), m_socket(-1)  {
  setup_server_sockets();
}

void HttpServer::setup_server_sockets() {
  m_status = HttpServerStatus::INVALID;

  this->m_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (this->m_socket == -1) {
    perror("Failed to create socket \n");
    return;
  }

  sockaddr_in local_address;
  local_address.sin_family = AF_INET;
  local_address.sin_port = htons(8080);
  local_address.sin_addr.s_addr = inet_addr("127.0.0.1");  

  if (bind(this->m_socket, (sockaddr*) &local_address, sizeof(local_address)) == - 1) {
    perror("failed to bind");
    close(this->m_socket);
    return;
  }

  if (listen(this->m_socket, MAX_CONNECTIONS) == -1) {
    perror("failed to listen on socket");
    close(this->m_socket);
    return;
  };
  
  m_status = HttpServerStatus::VALID;
}
void HttpServer::start() const {
  if (m_status == HttpServerStatus::INVALID || m_status == HttpServerStatus::ASLEEP) 
    throw std::runtime_error("server must be in valid state to start\n");

  while ( true ) {
    int client_socket = accept(this->m_socket, NULL, NULL);

    if ( client_socket == -1) 
      continue;
     
    char raw_http_request[MAX_REQUEST_SIZE_BYTES];
    memset(raw_http_request, 0, MAX_REQUEST_SIZE_BYTES); 

    if ( recv(client_socket, raw_http_request, MAX_REQUEST_SIZE_BYTES, 0) == -1 ) {
      perror("Failed to receive HTTP REQUEST : ");
      continue;
    }

    HttpRequest http_request = parse_request(raw_http_request);
    launch_request_handler(http_request, client_socket);
  }
}


void HttpServer::send_error(int32_t connection_socket, const std::string& error_description, uint32_t error_code) const {
  HttpResponse response;
  response.add_status_line({"HTTP/1.1", error_description, error_code})
    .add_header("Access-Control-Allow-Origin", "*");

  send(connection_socket, response.content.c_str(), response.content.size(), 0); 

  close(connection_socket);
};

// rather use a hashmap O(1) instant access
void HttpServer::launch_request_handler(const HttpRequest& request, int32_t client_socket) const {
  if (m_urls == nullptr) {
    this->send_error(client_socket, "Not Found", 404);
    return;
  }

  HttpRouter* handler = m_urls->retrieve_handler(request.request_line.target);

  if (handler == nullptr) {
    this->send_error(client_socket, "Not Found", 404);
    return ;
  }

  handler->handle(request, client_socket);
};


HttpRequestLine  HttpServer::parse_request_line(const std::string& request_line) const {
  const int32_t METHOD{0};
  const int32_t REQUEST_TARGET{1};
  const int32_t VERSION{2};

  auto request_line_fields = parsing::split_string(request_line, ' ');

  return {
   get_http_method(request_line_fields[METHOD]), 
   request_line_fields[REQUEST_TARGET], 
   parsing::strip_string_from(request_line_fields[VERSION], '\r'), 
  };
};



HttpRequest HttpServer::parse_request(const char* raw_data) const {
  const std::string OPTIONS_BODY_SEPARATOR{"\r"};
  HttpRequest http_request{};

  auto parsed_lines = parsing::split_string(raw_data, '\n');
  http_request.request_line = parse_request_line(parsed_lines[0]);

  const auto& headers_start = parsed_lines.begin() + 1;
  const auto& headers_end = std::find(parsed_lines.begin()+1 , parsed_lines.end(), OPTIONS_BODY_SEPARATOR);
 
  const auto& body_start = headers_end + 1;
  const auto& body_end = parsed_lines.end();

  http_request.headers = parse_headers(headers_start, headers_end);
  http_request.body = parse_body(body_start, body_end);

  return http_request;
};


template<typename StartIterator, typename EndIterator>
HttpOptions  HttpServer::parse_headers(StartIterator data_start, EndIterator data_end) const {
  auto extract_field_and_value = [](const std::string& line) {
    const int32_t split_index = line.find(": ") ;
    const int32_t end = line.size() - split_index;
    const int32_t separator_length = 2;

    return std::pair<std::string, std::string> {
      line.substr(0,split_index),
      line.substr(split_index + separator_length, end)
    };
  };

  HttpOptions headers{};
  
  for ( ;data_start != data_end; data_start++)  {
    std::string line = parsing::strip_string_from(*data_start, '\r');
    const auto field_value_tuple = extract_field_and_value(line);
    headers.fields[field_value_tuple.first] = field_value_tuple.second;
  }

  return headers;
};

template<typename StartIterator, typename EndIterator>
std::string  HttpServer::parse_body(StartIterator start_data, EndIterator end_data) const {
  std::string body_data;
  for (; start_data != end_data ; start_data++ ) {
    body_data += *start_data;
  }
  return body_data;
}; 

