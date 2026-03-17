cmake_minimum_required(VERSION 3.26)
project("Http Server")


set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Debug)
include_directories(Include/)
include_directories(HttpRouters/)


add_subdirectory(Parsing)

add_library(HttpServerLib HttpServer.cpp)
add_executable(main example-main.cpp)

target_link_libraries(main HttpServerLib ParsingLib) 

