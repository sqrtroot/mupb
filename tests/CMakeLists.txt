cmake_minimum_required(VERSION 3.6)

project(mupb_test)
find_package(Catch2 REQUIRED)


#set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=*)

file(GLOB_RECURSE test_sources src/*.cpp)
add_executable(unit_tests main.cpp ${test_sources} src/types_wire_type.cpp)

target_include_directories(unit_tests PRIVATE "../include")

target_compile_features(unit_tests PRIVATE cxx_std_17)
target_compile_options(unit_tests PRIVATE --coverage)
target_compile_options(unit_tests PRIVATE -g -Wall -Wextra -pedantic)

target_link_libraries(unit_tests Catch2::Catch2)
target_link_options(unit_tests PRIVATE --coverage)

include(CTest)
include(Catch)
set(CTEST_OUTPUT_ON_FAILURE true)
catch_discover_tests(unit_tests)
#add_custom_command(OUTPUT coverage/coverage.info
#  COMMAND ./unit_tests || true
#  COMMAND mkdir -p coverage
#  COMMAND lcov -c -d CMakeFiles/unit_tests.dir -o coverage/coverage.info --exclude "'/usr/*'"
#  DEPENDS unit_tests)
#
#add_custom_target(coverage
#  COMMAND lcov --summary coverage/coverage.info
#  DEPENDS coverage/coverage.info
#  )
#add_custom_target(html
#  COMMAND mkdir -p coverage/html/
#  COMMAND genhtml coverage/coverage.info -o coverage/html
#  DEPENDS coverage/coverage.info)
##
