cmake_minimum_required(VERSION 3.25)
project(counter_cpp_fleck LANGUAGES C CXX)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  include(CTest)
  find_package(Sunfleck 1.0.0 CONFIG REQUIRED)
endif()

add_executable(counter_cpp_fleck main.cpp)
set_target_properties(counter_cpp_fleck PROPERTIES WIN32_EXECUTABLE "${WIN32}")
find_package(Threads REQUIRED)
target_link_libraries(counter_cpp_fleck PRIVATE Sunfleck::sunfleck Threads::Threads)
target_compile_features(counter_cpp_fleck PRIVATE c_std_11)
target_compile_features(counter_cpp_fleck PRIVATE cxx_std_20)

set(page "${CMAKE_CURRENT_SOURCE_DIR}/counter.fleck")
set(generated "${CMAKE_CURRENT_BINARY_DIR}/counter_page.c")
set(bindings "${CMAKE_CURRENT_BINARY_DIR}/counter_bindings.hpp")
add_custom_command(OUTPUT "${generated}" "${bindings}" "${generated}.fleckmap"
  COMMAND Sunfleck::fleckc --symbol load_counter --source-map "${generated}.fleckmap" "${page}" "${generated}"
  COMMAND Sunfleck::fleckc --format cpp --symbol load_counter "${page}" "${bindings}"
  DEPENDS Sunfleck::fleckc "${page}"
  VERBATIM)
target_sources(counter_cpp_fleck PRIVATE "${generated}" "${bindings}")
target_include_directories(counter_cpp_fleck PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
target_compile_definitions(counter_cpp_fleck PRIVATE SUNFLECK_COUNTER_SOURCE_MAP="${generated}.fleckmap")

if(BUILD_TESTING)
  add_test(NAME counter_cpp_fleck COMMAND counter_cpp_fleck --test)
endif()
