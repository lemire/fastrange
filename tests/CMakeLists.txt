# The tests are the only part of the project that needs a compiler.
enable_language(C)
enable_language(CXX)

add_executable(fastrange_c_test c/tests.c)
target_link_libraries(fastrange_c_test PRIVATE fastrange::fastrange)
set_target_properties(fastrange_c_test PROPERTIES
  C_STANDARD 99
  C_STANDARD_REQUIRED ON
  C_EXTENSIONS OFF)

add_executable(fastrange_cpp_test cpp/tests.cpp)
target_link_libraries(fastrange_cpp_test PRIVATE fastrange::fastrange)
set_target_properties(fastrange_cpp_test PROPERTIES
  CXX_STANDARD 11
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)

foreach(target fastrange_c_test fastrange_cpp_test)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4)
  else()
    target_compile_options(${target} PRIVATE -Wall -Wextra)
  endif()
  add_test(NAME ${target} COMMAND ${target})
endforeach()
