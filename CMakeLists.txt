cmake_minimum_required(VERSION 3.20)
project(AevumEngine CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES "src/opengl/glad.c")

add_executable(AevumEngine ${SOURCES})

target_include_directories(AevumEngine PRIVATE
    src/
    Libraries/include/
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(AevumEngine PRIVATE
    OpenGL::GL
    glfw
    dl
)

set_target_properties(AevumEngine PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)