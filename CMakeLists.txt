cmake_minimum_required(VERSION 3.29)
project(CCUtilities VERSION 1.0 LANGUAGES CXX)

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR})
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

add_library(compiler_args INTERFACE)
target_compile_features(compiler_args INTERFACE cxx_std_20)
target_compile_options(compiler_args INTERFACE /O2)

# target_compile_options(compiler_args INTERFACE /O2)
add_library(CCUtilities CCUtilities.cpp)
target_link_libraries(CCUtilities compiler_args)
target_include_directories(MyLibrary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
