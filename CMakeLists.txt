cmake_minimum_required(VERSION 3.20)
project(my_http_server CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ── Release flags ──────────────────────────────────────────────────────────────
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(
  -Wall -Wextra -Wpedantic
  $<$<CONFIG:Release>:-O2>
  $<$<CONFIG:Debug>:-g>
  $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)
add_link_options($<$<CONFIG:Debug>:-fsanitize=address,undefined>)

# ── Sources ────────────────────────────────────────────────────────────────────
file(GLOB_RECURSE SERVER_SOURCES CONFIGURE_DEPENDS src/*.cpp)
list(FILTER SERVER_SOURCES EXCLUDE REGEX ".*/main\\.cpp$")

add_executable(server src/main.cpp ${SERVER_SOURCES})
target_include_directories(server PRIVATE src)

# ── Tests ──────────────────────────────────────────────────────────────────────
enable_testing()
add_subdirectory(tests)
