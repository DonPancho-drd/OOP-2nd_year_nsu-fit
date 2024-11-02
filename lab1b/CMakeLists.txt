cmake_minimum_required(VERSION 3.10)

set(SOURCES
  circular_buf.cpp
)

set(HEADERS
  circular_buf.h
)
add_library(circular_buf STATIC ${SOURCES} ${HEADERS})
set_property(TARGET circular_buf PROPERTY CXX_STANDARD 20)

set(TEST_SOURCES circular_buf_test.cpp)
add_executable(circular_buf_test ${TEST_SOURCES})
target_link_libraries(circular_buf_test PRIVATE GTest::gtest_main circular_buf)

include(GoogleTest)

gtest_discover_tests(circular_buf_test)
