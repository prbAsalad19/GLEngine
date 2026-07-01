cmake_minimum_required(VERSION 3.20)
project(AevumEngine CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES "src/opengl/glad.c")
list(APPEND SOURCES "src/core/ecs/flecs.c")

# ImGui sources
list(APPEND SOURCES
    "Libraries/include/imgui-docking/imgui.cpp"
    "Libraries/include/imgui-docking/imgui_draw.cpp"
    "Libraries/include/imgui-docking/imgui_tables.cpp"
    "Libraries/include/imgui-docking/imgui_widgets.cpp"
    "Libraries/include/imgui-docking/backends/imgui_impl_glfw.cpp"
    "Libraries/include/imgui-docking/backends/imgui_impl_opengl3.cpp"
)

set_source_files_properties(
    "src/core/ecs/flecs.c"
    PROPERTIES LANGUAGE C
)

add_executable(AevumEngine ${SOURCES})

target_include_directories(AevumEngine PRIVATE
    src/
    Libraries/include/
    Libraries/include/imgui-docking/
    Libraries/include/imgui-docking/backends/
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