cmake_minimum_required(VERSION 3.20)
project(dprot_pract2_5 C)

set(CMAKE_C_STANDARD 99)
find_package(OpenSSL REQUIRED)
add_executable(dprot_pract2_5 main.c)
target_link_libraries(dprot_pract2_5 PRIVATE OpenSSL::SSL)