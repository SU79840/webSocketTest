cmake_minimum_required(VERSION 2.8)

project(webSocketTest)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -D__STDC_CONSTANT_MACROS")


set     (HEADERS
        EasyWsClient.hpp
        )

set     (SOURCES
        EasyWsClient.cpp
        )

set (TESTSRCS main.cpp)
add_executable(${PROJECT_NAME}
               ${HEADERS}
               ${SOURCES}
               ${TESTSRCS})
