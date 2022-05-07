# CMakeList.txt : CMake project for SimpleGame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("SimpleGame" C)

find_package(SDL2 REQUIRED)
find_package(sdl2-image REQUIRED)
find_package(sdl2-mixer REQUIRED)

# Add source to this project's executable.
add_executable (SimpleGame   "src/main.c" "src/init.c" "src/draw.c" "src/input.c"  "src/stage.c" "src/sound.c" "src/text.c")

target_include_directories(SimpleGame PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/include")

target_include_directories(SimpleGame PUBLIC ${SDL2_INCLUDE_DIRS})

target_link_libraries(SimpleGame PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_link_libraries(SimpleGame PRIVATE SDL2::SDL2_image)
target_link_libraries(SimpleGame PUBLIC SDL2::SDL2_mixer) # ensure sdl2-mixer "extensions" are installed for .ogg: https://www.reddit.com/r/cataclysmdda/comments/glxgtb/fix_for_sound_problem_when_compiling_in_windows/

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(SimpleGame copy_assets)