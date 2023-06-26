cmake_minimum_required (VERSION 3.0)
include(ExternalProject)

project (flywheel)

set(CMAKE_CXX_STANDARD 20)

include (FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git)
FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git)
FetchContent_MakeAvailable(glfw fmt)

set (SKIA_SRC ${PROJECT_SOURCE_DIR}/skia)

add_library (skia STATIC IMPORTED GLOBAL)
set_target_properties (skia PROPERTIES
    IMPORTED_LOCATION ${SKIA_SRC}/out/Static/skia.lib
    INTERFACE_INCLUDE_DIRECTORIES ${SKIA_SRC})

add_executable(flywheel src/main.cpp)
target_link_libraries(flywheel glfw fmt skia opengl32)
