# Fetching GLAD with CMake

```
FetchContent_Declare(glad GIT_REPOSITORY https://github.com/dav1dde/glad.git GIT_TAG glad2)
FetchContent_MakeAvailable(glad)
 
add_subdirectory (${CMAKE_BINARY_DIR}/_deps/glad-src/cmake)
glad_add_library (glad SHARED API gl:core=4.6)

add_executable${PROJECT_NAME} ${SOURCES})
target_link_directories (${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/gladsources/include)
target_link_libraries(${PROJECT_NAME} glad)
```
