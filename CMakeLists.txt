cmake_minimum_required(VERSION 3.29.6)

project(OpenGl_Triangle)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_SOURCE_DIR}/ext/glfw)
add_subdirectory(${CMAKE_SOURCE_DIR}/ext/glad)
add_subdirectory(${CMAKE_SOURCE_DIR}/ext/glm)

set(EXTERNAL glfw glad glm)

add_executable(${PROJECT_NAME} src/main.cpp)

include_directories(${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} GL dl ${EXTERNAL})
