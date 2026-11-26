cmake_minimum_required(VERSION 3.25)
project(counter_cpp_code LANGUAGES C CXX)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  include(CTest)
  find_package(Sunfleck 1.0.0 CONFIG REQUIRED)
endif()

add_executable(counter_cpp_code main.cpp)
set_target_properties(counter_cpp_code PROPERTIES WIN32_EXECUTABLE "${WIN32}")
target_link_libraries(counter_cpp_code PRIVATE Sunfleck::sunfleck)
target_compile_features(counter_cpp_code PRIVATE c_std_11)
target_compile_features(counter_cpp_code PRIVATE cxx_std_20)

if(BUILD_TESTING)
  add_test(NAME counter_cpp_code COMMAND counter_cpp_code --test)
endif()
