cmake_minimum_required(VERSION 3.25)
project(showcase_cpp_fleck LANGUAGES C CXX)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  include(CTest)
  find_package(Sunfleck 1.0.0 CONFIG REQUIRED)
endif()
set(page "${CMAKE_CURRENT_SOURCE_DIR}/showcase.fleck")
set(generated "${CMAKE_CURRENT_BINARY_DIR}/showcase_page.c")
set(bindings "${CMAKE_CURRENT_BINARY_DIR}/showcase_bindings.hpp")
add_custom_command(OUTPUT "${generated}" "${bindings}"
  COMMAND Sunfleck::fleckc --symbol load_showcase "${page}" "${generated}"
  COMMAND Sunfleck::fleckc --format cpp --symbol load_showcase "${page}" "${bindings}"
  DEPENDS Sunfleck::fleckc "${page}"
  VERBATIM)
add_library(showcase_cpp_fleck_page STATIC "${generated}" "${bindings}")
target_include_directories(showcase_cpp_fleck_page INTERFACE "${CMAKE_CURRENT_BINARY_DIR}")
target_link_libraries(showcase_cpp_fleck_page PRIVATE Sunfleck::sunfleck)

add_executable(showcase_cpp_fleck main.cpp showcase_model.cpp)
target_link_libraries(showcase_cpp_fleck PRIVATE showcase_cpp_fleck_page)
set_target_properties(showcase_cpp_fleck PROPERTIES WIN32_EXECUTABLE "${WIN32}")
target_link_libraries(showcase_cpp_fleck PRIVATE Sunfleck::sunfleck)
target_compile_features(showcase_cpp_fleck PRIVATE c_std_11)
if(BUILD_TESTING)
  add_test(NAME showcase_cpp_fleck COMMAND showcase_cpp_fleck --test)
endif()
target_compile_features(showcase_cpp_fleck PRIVATE cxx_std_20)
