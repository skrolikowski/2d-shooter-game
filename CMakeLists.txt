cmake_minimum_required(VERSION 3.20)
project(2d-shooter-game CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# if(NOT CMAKE_BUILD_TYPE)
#   set(CMAKE_BUILD_TYPE RelWithDebInfo)
# endif()

## -- BEGIN FETCH CONTENT --
include(FetchContent)
## SDL3
FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG main
    GIT_SHALLOW TRUE)

## SDL3_image: PNG loading.
## Turn off formats that need extra libraries.
set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_JXL  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_TIF  OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)
FetchContent_Declare(SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG main
    GIT_SHALLOW TRUE)

## SDL3_ttf: text.
## Vendored builds FreeType and HarfBuzz from its submodules.
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
FetchContent_Declare(SDL3_ttf
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
  GIT_TAG main
  GIT_SHALLOW TRUE)

FetchContent_Declare(nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
  GIT_SHALLOW TRUE)

FetchContent_MakeAvailable(SDL3 SDL3_image SDL3_ttf nlohmann_json)
## -- END FETCH CONTENT --

# Compile every .cpp under src/ into a static library.
# CONFIGURE_DEPENDS re-globs on each build, so new files are picked up automatically.

# file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
# add_library(src STATIC ${SRC_FILES})

# manually..
add_library(src STATIC
  src/App.cpp
  src/Util/Input.cpp
  src/Util/Texture.cpp
  src/Util/TextureManager.cpp
  src/World/World.cpp
  src/World/Bunker.cpp
  src/World/FormationColumn.cpp
  src/World/Formation.cpp
  src/World/Entity.cpp
  src/World/Bullet.cpp
  src/World/Player.cpp
  src/GameStates/GameOverState.cpp
  src/GameStates/TitleState.cpp
  src/GameStates/PlayingState.cpp
  src/GameStates/PlayerDyingState.cpp)

target_include_directories(src PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(src PUBLIC SDL3::SDL3 SDL3_image::SDL3_image SDL3_ttf::SDL3_ttf nlohmann_json::nlohmann_json)

# Scratch executable for trying things out: ./build/main
add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

target_link_libraries(${PROJECT_NAME} PRIVATE src)


# Copy assets next to the binary
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)