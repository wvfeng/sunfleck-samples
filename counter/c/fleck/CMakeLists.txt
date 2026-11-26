cmake_minimum_required(VERSION 3.25)
project(counter_c_fleck LANGUAGES C CXX)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  include(CTest)
  find_package(Sunfleck 1.0.0 CONFIG REQUIRED)
endif()

add_executable(counter_c_fleck main.c)
set_target_properties(counter_c_fleck PROPERTIES WIN32_EXECUTABLE "${WIN32}")
target_link_libraries(counter_c_fleck PRIVATE Sunfleck::sunfleck)
target_compile_features(counter_c_fleck PRIVATE c_std_11)

set(page "${CMAKE_CURRENT_SOURCE_DIR}/counter.fleck")
set(generated "${CMAKE_CURRENT_BINARY_DIR}/counter_page.c")
add_custom_command(OUTPUT "${generated}" "${generated}.fleckmap"
  COMMAND Sunfleck::fleckc --symbol load_counter --source-map "${generated}.fleckmap" "${page}" "${generated}"
  DEPENDS Sunfleck::fleckc "${page}"
  VERBATIM)
target_sources(counter_c_fleck PRIVATE "${generated}")
target_compile_definitions(counter_c_fleck PRIVATE SUNFLECK_COUNTER_SOURCE_MAP="${generated}.fleckmap")

if(BUILD_TESTING)
  add_test(NAME counter_c_fleck COMMAND counter_c_fleck --test)
endif()
