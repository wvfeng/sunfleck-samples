add_subdirectory(c/code)
add_subdirectory(c/fleck)
add_subdirectory(cpp/code)
add_subdirectory(cpp/fleck)

if(BUILD_TESTING)
  add_executable(showcase_c_parity
    ../tests/showcase_parity.c c/code/layout.c showcase.c c/fleck/fleck_handles.c)
  target_include_directories(showcase_c_parity PRIVATE .)
  target_link_libraries(showcase_c_parity PRIVATE Sunfleck::sunfleck showcase_c_fleck_page)
  add_test(NAME showcase_c_parity COMMAND showcase_c_parity)

  add_executable(showcase_cpp_parity
    ../tests/showcase_cpp_parity.cpp cpp/code/layout.cpp showcase.c cpp/fleck/showcase_model.cpp)
  target_include_directories(showcase_cpp_parity PRIVATE . cpp/fleck)
  target_link_libraries(showcase_cpp_parity PRIVATE Sunfleck::sunfleck showcase_cpp_fleck_page)
  add_test(NAME showcase_cpp_parity COMMAND showcase_cpp_parity)

  if(MSVC)
    target_compile_options(showcase_c_parity PRIVATE /utf-8)
    target_compile_options(showcase_cpp_parity PRIVATE /utf-8)
  endif()
endif()
