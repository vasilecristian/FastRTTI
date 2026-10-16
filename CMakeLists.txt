# FastRTTI, a header-only library.
#
# Open this folder in Visual Studio (File > Open > Folder), VS Code or CLion and
# pick a preset from CMakePresets.json, or from a command line:
#
#   cmake --preset windows-release
#   cmake --build --preset windows-release
#   ctest --preset windows-release
#
# Other projects can add_subdirectory() this folder and link FastRTTI::FastRTTI.
cmake_minimum_required(VERSION 3.10...3.21)
project(FastRTTI CXX)

add_library(FastRTTI INTERFACE)
add_library(FastRTTI::FastRTTI ALIAS FastRTTI)
target_include_directories(FastRTTI INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# The tests are built by default only when FastRTTI is the top-level project.
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
	set(FASTRTTI_BUILD_TESTS_DEFAULT ON)
else()
	set(FASTRTTI_BUILD_TESTS_DEFAULT OFF)
endif()
option(FASTRTTI_BUILD_TESTS "Build the FastRTTI tests and benchmarks" ${FASTRTTI_BUILD_TESTS_DEFAULT})

if(FASTRTTI_BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif()
