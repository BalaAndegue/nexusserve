include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.cpp)
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

add_executable(server_tests ${TEST_SOURCES} ${LIB_SOURCES})
target_include_directories(server_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(server_tests PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(server_tests)
