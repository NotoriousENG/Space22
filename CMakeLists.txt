# CMakeList.txt : CMake project for SimpleGame, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("SimpleGame" C)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_compile_definitions("EMSCRIPTEN")
    set(USE_FLAGS "-s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 --preload-file assets --preload-file assets/highscores.bin --use-preload-plugins --shell-file SimpleGame.html")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
else()
    find_package(SDL2 REQUIRED)
    find_package(sdl2-image REQUIRED)
    find_package(sdl2-mixer REQUIRED)
endif()

# Add source to this project's executable.
add_executable (SimpleGame   "src/main.c" "src/init.c" "src/draw.c" "src/input.c"  "src/stage.c" "src/sound.c" "src/text.c" "src/highscores.c" "src/background.c" "src/title.c" "src/util.c")

target_include_directories(SimpleGame PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/include")

target_include_directories(SimpleGame PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(SimpleGame PRIVATE ${SDL2_LIBRARIES})

target_link_libraries(SimpleGame PRIVATE ${SDL2_IMAGE_LIBRARIES})
target_link_libraries(SimpleGame PRIVATE ${SDL2_MIXER_LIBRARIES}) # ensure sdl2-mixer "extensions" are installed for .ogg: https://www.reddit.com/r/cataclysmdda/comments/glxgtb/fix_for_sound_problem_when_compiling_in_windows/

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)
add_dependencies(SimpleGame copy_assets)

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    add_custom_target(copy_html
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_LIST_DIR}/web/SimpleGame.html ${CMAKE_CURRENT_BINARY_DIR}/SimpleGame.html
    )
    add_dependencies(SimpleGame copy_html)
endif()