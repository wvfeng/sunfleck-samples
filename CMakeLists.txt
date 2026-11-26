cmake_minimum_required(VERSION 3.25)
project(SunfleckSamples LANGUAGES C CXX)
include(CTest)
find_package(Sunfleck 1.0.0 CONFIG REQUIRED)

add_subdirectory(counter)
add_subdirectory(showcase)
