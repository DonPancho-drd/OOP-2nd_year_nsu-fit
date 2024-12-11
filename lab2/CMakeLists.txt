cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Найти Google Test
find_package(GTest REQUIRED)

set(SOURCES
  "src/game.cpp" "src/strategy.cpp" "src/strategy_factory.cpp" "src/main.cpp" "src/args_parser.cpp"
)

set(HEADERS
  "include/game.h" "include/strategy.h" "include/strategy_factory.h" "include/args_parser.h"
)

add_executable(game ${SOURCES} ${HEADERS})

set_property(TARGET game PROPERTY CXX_STANDARD 20)
target_include_directories(game PUBLIC "include")

# set(TEST_SOURCES
# "src/game.cpp" "src/strategy.cpp" "src/strategy_factory.cpp" "src/main.cpp"
#     "test/game_test.cpp")
# add_executable(game_test ${TEST_SOURCES} ${HEADERS})
# target_link_libraries(game_test PRIVATE GTest::gtest)
# set_property(TARGET game_test PROPERTY CXX_STANDARD 20)
# target_include_directories(game_test PUBLIC "include")
#
# include(GoogleTest)
# gtest_discover_tests(game_test)
