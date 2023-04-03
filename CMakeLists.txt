cmake_minimum_required(VERSION 3.0)

project(zip_iterables VERSION 1.0.0 LANGUAGES CXX)

add_library(zip_iterables INTERFACE zip_iterables.hpp)
add_library(zip_iterables::zip_iterables ALIAS zip_iterables)

target_include_directories(zip_iterables INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
