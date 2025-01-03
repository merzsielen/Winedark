﻿cmake_minimum_required (VERSION 3.22)

project (winedark LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BASE_SRCS
    "src/rendering/camera.cpp"
    "src/rendering/camera.h"
    "src/rendering/renderer.cpp"
    "src/rendering/renderer.h"
    "src/rendering/shader.cpp"
    "src/rendering/shader.h"
    "src/rendering/texture.cpp"
    "src/rendering/texture.h"
    "src/rendering/textureatlas.cpp"
    "src/rendering/textureatlas.h"
    "src/util/geometry.cpp"
    "src/util/geometry.h"
    "src/util/polygons.h"
    "src/world/octree.cpp"
    "src/world/octree.h"
    "src/main.cpp"
    )

add_executable (winedark ${BASE_SRCS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)

add_dependencies(winedark copy_assets)

# add_subdirectory(libs/freetype-2.12.1)
add_subdirectory(libs/glfw-3.3.8)
add_subdirectory(libs/glad)
add_subdirectory(libs/glm-0.9.9.8)
add_subdirectory(libs/stb_image)

if(NOT DEFINED CMAKE_SUPPRESS_DEVELOPER_WARNINGS)
     set(CMAKE_SUPPRESS_DEVELOPER_WARNINGS 1 CACHE INTERNAL "No dev warnings")
endif()

target_link_libraries(winedark glfw glad glm stb_image) # freetype)
