cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(bgfx-sdf)

# find src/* -name "*.cpp" -printf "%p\n"
add_executable(bgfx-sdf "src/main.cpp")

target_compile_features(bgfx-sdf PUBLIC cxx_std_17)
target_compile_options(bgfx-sdf PUBLIC
	# $<$<CXX_COMPILER_ID:MSVC>:/W4> # /WX
	# $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic> # -Werror
)

add_subdirectory(libs/bgfx.cmake)
add_subdirectory(libs/glfw)
add_subdirectory(libs/freetype)

# disable glfw tests, examples and documentation
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

include_directories(bgfx-sdf
	SYSTEM "libs/bgfx.cmake/bgfx/include" "libs/glfw/include" "libs/freetype/include"
)

target_link_libraries(bgfx-sdf
	bgfx glfw freetype
)
