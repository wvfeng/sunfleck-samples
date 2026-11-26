cmake_minimum_required(VERSION 3.25)
project(showcase_c_fleck LANGUAGES C CXX)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  include(CTest)
  find_package(Sunfleck 1.0.0 CONFIG REQUIRED)
endif()
set(page "${CMAKE_CURRENT_SOURCE_DIR}/showcase.fleck")
set(generated "${CMAKE_CURRENT_BINARY_DIR}/showcase_page.c")
add_custom_command(OUTPUT "${generated}"
  COMMAND Sunfleck::fleckc --symbol load_showcase "${page}" "${generated}"
  DEPENDS Sunfleck::fleckc "${page}"
  VERBATIM)
add_library(showcase_c_fleck_page STATIC "${generated}")
target_link_libraries(showcase_c_fleck_page PRIVATE Sunfleck::sunfleck)

add_executable(showcase_c_fleck main.c ../../showcase.c fleck_handles.c)
target_include_directories(showcase_c_fleck PRIVATE ../..)
target_link_libraries(showcase_c_fleck PRIVATE showcase_c_fleck_page)
set_target_properties(showcase_c_fleck PROPERTIES WIN32_EXECUTABLE "${WIN32}")
target_link_libraries(showcase_c_fleck PRIVATE Sunfleck::sunfleck)
target_compile_features(showcase_c_fleck PRIVATE c_std_11)
if(BUILD_TESTING)
  add_test(NAME showcase_c_fleck COMMAND showcase_c_fleck --test)
endif()
