# Test suite for FastRTTI.
#
# Each executable covers one build configuration the library has to work in, which
# is why the suite is not a single binary: a smaller chain size, the compiler's own
# RTTI switched off and a type crossing a shared library boundary each need their
# own compile settings.
#
# It is built from the CMakeLists.txt at the repository root (see the presets in
# CMakePresets.json), or on its own:
#
#   cmake -S tests -B build/tests -G Ninja
#   cmake --build build/tests
#   ctest --test-dir build/tests --output-on-failure
#
# In the standalone build, FASTRTTI_INCLUDE_DIR selects the header under test, so
# another version of FastRTTI/RTTI.hpp can be checked against the same suite.
cmake_minimum_required(VERSION 3.10...3.21)
project(FastRTTITests CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Built from the repository root, the tests link the FastRTTI::FastRTTI target it
# defines, the way another project would. Built on their own, they define the same
# target over FASTRTTI_INCLUDE_DIR.
if(NOT TARGET FastRTTI::FastRTTI)
	set(FASTRTTI_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include" CACHE PATH
		"Directory that contains FastRTTI/RTTI.hpp")
	add_library(FastRTTI INTERFACE)
	add_library(FastRTTI::FastRTTI ALIAS FastRTTI)
	target_include_directories(FastRTTI INTERFACE "${FASTRTTI_INCLUDE_DIR}")
endif()

# Executables and the plugin share one directory, so Windows finds the DLL.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(FASTRTTI_TESTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/UnitTests")

enable_testing()

function(fastrtti_configure_target name)
	target_link_libraries(${name} PRIVATE FastRTTI::FastRTTI)
	target_include_directories(${name} PRIVATE "${FASTRTTI_TESTS_DIR}")
	if(MSVC)
		target_compile_options(${name} PRIVATE /W4)
	else()
		target_compile_options(${name} PRIVATE -Wall -Wextra)
	endif()
endfunction()

# A test executable, built with the suite and run by CTest.
function(fastrtti_add_test name)
	add_executable(${name} ${ARGN})
	fastrtti_configure_target(${name})
	add_test(NAME ${name} COMMAND ${name})
endfunction()

# A test whose first assertion is that it compiles at all. The target is left out
# of the normal build and CTest builds it, so a header that does not compile
# shows up as one failed test instead of stopping the whole suite from building.
function(fastrtti_add_compile_test name)
	add_executable(${name} EXCLUDE_FROM_ALL ${ARGN})
	fastrtti_configure_target(${name})

	add_test(NAME ${name}Compiles
		COMMAND "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}" --target ${name} --config "$<CONFIG>")
	set_tests_properties(${name}Compiles PROPERTIES
		FIXTURES_SETUP ${name}Build
		RESOURCE_LOCK BuildTree)

	add_test(NAME ${name}Runs COMMAND ${name})
	set_tests_properties(${name}Runs PROPERTIES FIXTURES_REQUIRED ${name}Build)
endfunction()

# Usage examples from the sample project, run as tests.
fastrtti_add_test(UsageTest UsageTest/UsageTest.cpp)

# Casts across every hierarchy shape: single, multiple, diamonds and templates.
fastrtti_add_test(CastingTests UnitTests/CastingTests.cpp)

# The chain during construction, per object, and on deletion through RTTI*.
fastrtti_add_test(ObjectLifetimeTests UnitTests/ObjectLifetimeTests.cpp)

# Type ids compared across two translation units of one executable.
fastrtti_add_test(TypeIdentityTests UnitTests/TypeIdentityTests.cpp UnitTests/TypeIdentityOtherUnit.cpp)

# BasicTypeRTTI, the wrapper for plain values.
fastrtti_add_test(BasicTypeTests UnitTests/BasicTypeTests.cpp)

# Objects built and cast on several threads at once.
find_package(Threads REQUIRED)
fastrtti_add_test(ThreadingTests UnitTests/ThreadingTests.cpp)
target_link_libraries(ThreadingTests PRIVATE Threads::Threads)

# Copies, moves and assignments must leave every object with its own chain.
fastrtti_add_test(CopySemanticsTests UnitTests/CopySemanticsTests.cpp)

# The debug name table only exists in a debug build.
fastrtti_add_test(ChainNamesTests UnitTests/ChainNamesTests.cpp)
target_compile_definitions(ChainNamesTests PRIVATE DEBUG)

# A hierarchy deeper than RTTI_CHAIN_MAX_SIZE, which the test sets itself.
fastrtti_add_test(ChainOverflowTests UnitTests/ChainOverflowTests.cpp)

# A type id has to mean the same thing on both sides of a shared library. Hidden
# visibility is what a real plugin is built with, and on Windows every DLL gets
# its own copy of a template static whatever the visibility.
add_library(CrossModulePlugin SHARED UnitTests/CrossModule/CrossModulePlugin.cpp)
fastrtti_configure_target(CrossModulePlugin)
target_compile_definitions(CrossModulePlugin PRIVATE CROSSMODULE_PLUGIN_BUILD)
set_target_properties(CrossModulePlugin PROPERTIES
	CXX_VISIBILITY_PRESET hidden
	VISIBILITY_INLINES_HIDDEN ON)

fastrtti_add_test(CrossModuleTests UnitTests/CrossModule/CrossModuleTests.cpp)
target_link_libraries(CrossModuleTests PRIVATE CrossModulePlugin)

# The header has to compile as the first and only include. libc++ otherwise pulls
# in most of the standard library transitively and hides a missing #include.
fastrtti_add_compile_test(SelfContainedHeaderTest UnitTests/SelfContainedHeaderTest.cpp)
target_compile_definitions(SelfContainedHeaderTest PRIVATE _LIBCPP_REMOVE_TRANSITIVE_INCLUDES)

# RTTI_ASSERT replaced by a statement-style project assert, the usual shape of one.
fastrtti_add_compile_test(StatementAssertMacroTest UnitTests/StatementAssertMacroTest.cpp)

# A debug build with the compiler's own RTTI switched off, as mobile and console
# toolchains build it.
fastrtti_add_compile_test(NoCompilerRttiTest UnitTests/NoCompilerRttiTest.cpp)
target_compile_definitions(NoCompilerRttiTest PRIVATE DEBUG)
if(MSVC)
	target_compile_options(NoCompilerRttiTest PRIVATE /GR-)
else()
	target_compile_options(NoCompilerRttiTest PRIVATE -fno-rtti)
endif()

# The FastRTTI against dynamic_cast benchmark, in the three builds the Visual Studio
# project had: FastRTTI alone with the compiler's RTTI off, dynamic_cast alone, and
# both side by side. Built to keep them compiling, but not CTest tests: they are
# slow and wait for a key press. Build them in Release to compare timings.
function(fastrtti_add_benchmark name)
	add_executable(${name} RTTIvsFASTRTTI/src/RTTITest.cpp)
	fastrtti_configure_target(${name})
	target_compile_definitions(${name} PRIVATE ${ARGN})
endfunction()

fastrtti_add_benchmark(FastRTTIBenchmark USE_FAST_RTTI)
if(MSVC)
	target_compile_options(FastRTTIBenchmark PRIVATE /GR-)
else()
	target_compile_options(FastRTTIBenchmark PRIVATE -fno-rtti)
endif()

fastrtti_add_benchmark(RealRTTIBenchmark USE_REAL_RTTI)
fastrtti_add_benchmark(BothRTTIBenchmark USE_FAST_RTTI USE_REAL_RTTI)
