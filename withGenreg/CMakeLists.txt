cmake_minimum_required(VERSION 3.22)

SET(CMAKE_INSTALL_PREFIX .)

project(pocitanieKostier
    DESCRIPTION "graphs"
    LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
  
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Wextra -D__UNIX__")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(spanningTreesRegular
    mainRegularGraphs.cpp
    processingRegularGraphs.cpp
    spanningTreeCounting.cpp
    main.cpp
    genreg.cpp
    readscd.cpp
    optmid.cpp
    cycleCounter.cpp
    cycleReducer.cpp
    cycleFinder.cpp
    )
target_link_libraries(spanningTreesRegular armadillo)

add_executable(cykly
    pocitanieCyklov.cpp
    spanningTreeCounting.cpp
    cycleFinder.cpp
    cycleReducer.cpp
    )
target_link_libraries(cykly armadillo)



install(TARGETS spanningTreesRegular cykly DESTINATION bin)
