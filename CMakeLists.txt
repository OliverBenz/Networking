cmake_minimum_required(VERSION 1.15)
project(Networking)

set(CMAKE_CXX_STANDARD 17)

set(ASIO_INCLUDE ${CMAKE_SOURCE_DIR}/lib/asio/include)

# Asio library
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${ASIO_INCLUDE})

target_link_libraries(asio INTERFACE pthread)


add_executable(test test.cpp)
target_link_libraries(test asio)
