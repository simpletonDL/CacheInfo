cmake_minimum_required(VERSION 3.20)
project(CacheInfo)

set(CMAKE_CXX_STANDARD 14)

add_executable(cacheLineSize cacheLineSize.cpp utils.cpp utils.h)

add_executable(cacheSize cacheSize.cpp utils.cpp utils.h)

add_executable(cacheAssoc cacheAssoc.cpp utils.cpp utils.h)
