cmake_minimum_required(VERSION 3.20)
project(GLEngine CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES "src/opengl/glad.c")

add_executable(GLEngine ${SOURCES})

target_include_directories(GLEngine PRIVATE
    src/
    Libraries/include/
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(GLEngine PRIVATE
    OpenGL::GL
    glfw
    dl
)

set_target_properties(GLEngine PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)