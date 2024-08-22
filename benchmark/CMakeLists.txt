project(bgfx-sdf-benchmark)

# disable optimizations
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

add_executable(bgfx-sdf-benchmark ./main.cpp ../libs/stb/stb_truetype.h
        GlyphGenerationBenchmark.cpp
        GlyphGenerationBenchmark.h
        GlyphRenderingBenchmark.cpp
        GlyphRenderingBenchmark.h
        GlyphPrecisionBenchmark.cpp
        GlyphPrecisionBenchmark.h)

include_directories(bgfx-sdf-benchmark SYSTEM ../libs/stb ../src)
target_link_libraries(bgfx-sdf-benchmark bgfx-sdf -static)

