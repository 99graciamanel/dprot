cmake_minimum_required(VERSION 3.20)
project(dprot_pract1 C)

set(CMAKE_C_STANDARD 99)
find_package(OpenSSL REQUIRED)
add_executable(dprot_pract1 main.c)
target_link_libraries(dprot_pract1 PRIVATE OpenSSL::SSL)
