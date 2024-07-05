project(bgfx-sdf-app)
add_executable(bgfx-sdf-app ../src/main.cpp ../libs/stb/stb_truetype.h)

include_directories(bgfx-sdf-app SYSTEM ../libs/stb ../src)
target_link_libraries(bgfx-sdf-app bgfx-sdf -static)

