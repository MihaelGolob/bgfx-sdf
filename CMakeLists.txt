cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(bgfx-sdf)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")
# find src/* -name "*.cpp" -printf "%p\n"
add_library(bgfx-sdf STATIC
#		src/main.cpp
		src/managers/FontManager.cpp
		src/managers/FontManager.h
		src/font_processing/CubeAtlas.cpp
		src/font_processing/CubeAtlas.h
		src/font_processing/utf8.cpp
		src/font_processing/utf8.h
		
		libs/stb/stb_truetype.h
		src/utilities.cpp
		src/managers/TextBufferManager.cpp
		src/managers/TextBufferManager.h
		src/utilities.h
		src/managers/InputManager.cpp
		src/managers/InputManager.h
		src/font_processing/RectanglePacker.cpp
		src/font_processing/RectanglePacker.h
		src/font_processing/AtlasRegion.cpp
		src/font_processing/AtlasRegion.h
		src/font_processing/FontInfo.h
		src/font_processing/GlyphInfo.h
		src/font_processing/TrueTypeFont.cpp
		src/font_processing/TrueTypeFont.h
		src/font_processing/NumberTypes.h
		src/font_processing/TextBuffer.cpp
		src/font_processing/TextBuffer.h
		src/font_processing/FontHandles.h
		src/sdf/sdf.h
		src/sdf/MsdfGenerator.cpp
		src/sdf/MsdfGenerator.h
		src/font_decomposition/Shape.cpp
		src/font_decomposition/Shape.h
		src/font_decomposition/Contour.cpp
		src/font_decomposition/Contour.h
		src/font_decomposition/Vector2.cpp
		src/font_decomposition/Vector2.h
		src/font_decomposition/EdgeSegment.h
		src/font_decomposition/EdgeSegment.cpp
		src/font_decomposition/EdgeHolder.cpp
		src/font_decomposition/EdgeHolder.h
		src/helper/EquationSolver.h
		src/helper/EquationSolver.cpp
)

target_compile_features(bgfx-sdf PUBLIC cxx_std_17)
target_compile_options(bgfx-sdf PUBLIC
	# $<$<CXX_COMPILER_ID:MSVC>:/W4> # /WX
	# $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic> # -Werror
)

add_subdirectory(libs/bgfx.cmake)
add_subdirectory(libs/glfw)
add_subdirectory(libs/freetype)
add_subdirectory(tests)
add_subdirectory(application)

#add_definitions(STB_TRUE_TYPE_IMPLEMENTATION)

# disable glfw tests, examples and documentation
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)

# command for compiling shaders
#add_custom_target(run_shader_compilation
#	COMMAND bash -c "make -C ${CMAKE_SOURCE_DIR}/src/shaders rebuild"
#	COMMENT "Compiling shaders"
#)

include_directories(bgfx-sdf
	SYSTEM 
		libs/bgfx.cmake/bgfx/include
		libs/bgfx.cmake/bx/include
		libs/bgfx.cmake/bimg/include
		
		libs/glfw/include
		libs/freetype/include
		
		libs/stb
)

target_link_libraries(bgfx-sdf
	bgfx bx bimg glfw freetype
)

#add_dependencies(bgfx-sdf run_shader_compilation)
