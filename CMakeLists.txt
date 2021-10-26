cmake_minimum_required(VERSION 3.20)
project(CacheInfo)

set(CMAKE_CXX_STANDARD 14)

add_executable(CacheInfo cacheLineSize.cpp)
