cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(bgfx-sdf)

# find src/* -name "*.cpp" -printf "%p\n"
add_executable(bgfx-sdf 
		src/main.cpp
		src/managers/FontManager.cpp
		src/managers/FontManager.h
		src/bgfx_example/cube_atlas.cpp
		src/bgfx_example/cube_atlas.h
		src/bgfx_example/utf8.cpp
		src/bgfx_example/utf8.h
		
		libs/stb/stb_truetype.h
		src/utilities.cpp
		src/managers/TextBufferManager.cpp
		src/managers/TextBufferManager.h
		src/utilities.h
)

target_compile_features(bgfx-sdf PUBLIC cxx_std_17)
target_compile_options(bgfx-sdf PUBLIC
	# $<$<CXX_COMPILER_ID:MSVC>:/W4> # /WX
	# $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic> # -Werror
)

add_subdirectory(libs/bgfx.cmake)
add_subdirectory(libs/glfw)
add_subdirectory(libs/freetype)

#add_definitions(STB_TRUE_TYPE_IMPLEMENTATION)

# disable glfw tests, examples and documentation
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

include_directories(bgfx-sdf
	SYSTEM 
		"libs/bgfx.cmake/bgfx/include"
		"libs/bgfx.cmake/bx/include"
		"libs/bgfx.cmake/bimg/include"
		
		"libs/glfw/include"
		"libs/freetype/include"
		
		libs/stb
)

target_link_libraries(bgfx-sdf
	bgfx bx bimg glfw freetype
)
