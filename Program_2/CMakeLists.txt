cmake_minimum_required(VERSION 3.22.0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Program_2)

include_directories(source)

set(SOURCES
    sources/main.cpp
    sources/client.cpp
    sources/receivehandler.cpp
)

set(HEADERS
    sources/client.h
    sources/receivehandler.h
)

add_executable(Program_2 ${SOURCES} ${HEADERS})
