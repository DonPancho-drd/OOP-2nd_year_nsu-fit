cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Найти Google Test
find_package(GTest REQUIRED)

set(HEADERS
    "include/config_parser.h"
    "include/factory.h"
    "include/converters.h"
    "include/sound_processor.h"
    "include/wav_file.h")
set(SOURCES
    "src/config_parser.cpp"
    "src/factory.cpp"
    "src/converters.cpp"
    "src/sound_processor.cpp"
    "src/wav_file.cpp"
    "src/main.cpp")
add_executable(sound_processor ${SOURCES} ${HEADERS})
set_property(TARGET sound_processor PROPERTY CXX_STANDARD 23)
target_include_directories(sound_processor PUBLIC "include")


# set(TEST_SOURCES
#     "src/config_parser.cpp"
#    "src/converters.cpp"
#    "src/sound_processor.cpp"
#    "src/wav_file.cpp"
#     "test.cpp")
# add_executable(test ${TEST_SOURCES} ${HEADERS})
# target_link_libraries(test PRIVATE GTest::gtest)
# set_property(TARGET test PROPERTY CXX_STANDARD 23)
# target_include_directories(test PUBLIC "include")
# include(GoogleTest)
# gtest_discover_tests(test)
