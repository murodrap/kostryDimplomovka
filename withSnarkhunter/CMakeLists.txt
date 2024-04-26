cmake_minimum_required(VERSION 3.22)

SET(CMAKE_INSTALL_PREFIX .)

project(pocitanieKostier
    DESCRIPTION "graphs"
    LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
  
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(spanningTreesRegular
    mainRegularGraphs.cpp
    processingRegularGraphs.cpp
    spanningTreeCounting.cpp)

add_executable(printer
    snarkhunterOutputPrinter.cpp)

install(TARGETS spanningTreesRegular printer DESTINATION bin)
