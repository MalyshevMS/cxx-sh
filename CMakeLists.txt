cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

set(PROJECT_NAME cxx-sh)

project(${PROJECT_NAME} CXX)

set(CXX_SH_INCLUDES 
    includes/cxx-sh/Core.hpp
    includes/cxx-sh/Parser/Parser.hpp
)

set(CXX_SH_SOURCES
    src/cxx-sh/Core.cpp
    src/cxx-sh/Parser/Parser.cpp
)

add_library(${PROJECT_NAME} STATIC ${CXX_SH_INCLUDES} ${CXX_SH_SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC includes)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)