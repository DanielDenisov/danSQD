cmake_minimum_required(VERSION 4.1)
project(danSQD)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(DRM     REQUIRED libdrm)
pkg_check_modules(WAYLAND REQUIRED wayland-client)
set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/imgui")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${IMGUI_DIR})
include_directories(${IMGUI_DIR}/backends)
include_directories(${DRM_INCLUDE_DIRS})
include_directories(${WAYLAND_INCLUDE_DIRS})

set(PROJECT_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        main.cpp
        config.h
        memory/memory.h
        memory/memory.cpp
        draw/WLLayerOverlay.h
        draw/ESP.h
        gamestate/GameState.cpp
        gamestate/GameState.h
        utils/gameUtil.h
        utils/localUtil.h
)

add_executable(danSQD ${PROJECT_SOURCES})

target_link_libraries(danSQD PRIVATE glfw OpenGL::GL ${DRM_LIBRARIES} ${WAYLAND_LIBRARIES})
