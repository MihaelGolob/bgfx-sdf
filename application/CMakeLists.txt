project(bgfx-sdf-app)
add_executable(bgfx-sdf-app ../src/main.cpp ../libs/stb/stb_truetype.h)

#add_custom_target(
#        rebuild_shaders
#        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_SOURCE_DIR}/src/shaders make rebuild
#        BYPRODUCTS ${CMAKE_SOURCE_DIR}/src/shaders/shader_output_file
#)
#
#add_custom_command(
#        TARGET bgfx-sdf-app
#        PRE_BUILD
#        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target rebuild_shaders
#)

include_directories(bgfx-sdf-app SYSTEM ../libs/stb ../src)
target_link_libraries(bgfx-sdf-app bgfx-sdf -static)

