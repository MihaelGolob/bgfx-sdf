project(bgfx-sdf-benchmark)
add_executable(bgfx-sdf-benchmark ./main.cpp ../libs/stb/stb_truetype.h)

include_directories(bgfx-sdf-benchmark SYSTEM ../libs/stb ../src)
target_link_libraries(bgfx-sdf-benchmark bgfx-sdf -static)

