### CMakeLists.txt

**Brief Overview**
This `CMakeLists.txt` file is used to configure and build a C++ project named "GLEngine". The project uses modern C++ features, specifically C++20. It includes source files from the `src` directory, links against OpenGL and GLFW libraries, and sets up directories for include paths.

**Table of Classes/Structs/Enums**
- None defined in this file.

**Detailed Methods Section**

#### cmake_minimum_required(VERSION 3.20)
- **Full Signature**: `cmake_minimum_required(VERSION 3.20)`
- **Return Type Explanation**: This function sets the minimum required version of CMake for the project to be built.
- **Internal Logic/Purpose**: Specifies that CMake 3.20 or newer is required.

#### project(GLEngine CXX C)
- **Full Signature**: `project(GLEngine CXX C)`
- **Return Type Explanation**: This function defines the name of the project and specifies the programming languages used.
- **Internal Logic/Purpose**: Defines the project name as "GLEngine" and indicates that both C++ and C are supported.

#### set(CMAKE_CXX_STANDARD 20)
- **Full Signature**: `set(CMAKE_CXX_STANDARD 20)`
- **Return Type Explanation**: This function sets the version of C++ standard to be used.
- **Internal Logic/Purpose**: Specifies that C++20 is the language standard for this project.

#### set(CMAKE_CXX_STANDARD_REQUIRED ON)
- **Full Signature**: `set(CMAKE_CXX_STANDARD_REQUIRED ON)`
- **Return Type Explanation**: This function makes the specified C++ standard mandatory.
- **Internal Logic/Purpose**: Ensures that C++20 is used and will fail if a different standard is provided.

#### file(GLOB_RECURSE SOURCES "src/*.cpp")
- **Full Signature**: `file(GLOB_RECURSE SOURCES "src/*.cpp")`
- **Return Type Explanation**: This function generates a list of source files.
- **Internal Logic/Purpose**: Recursively finds all `.cpp` files in the `src` directory and appends them to the `SOURCES` variable.

#### list(APPEND SOURCES "src/opengl/glad.c")
- **Full Signature**: `list(APPEND SOURCES "src/opengl/glad.c")`
- **Return Type Explanation**: This function appends an item to a list.
- **Internal Logic/Purpose**: Adds the `glad.c` file from the `src/opengl` directory to the `SOURCES` variable.

#### add_executable(GLEngine ${SOURCES})
- **Full Signature**: `add_executable(GLEngine ${SOURCES})`
- **Return Type Explanation**: This function defines an executable target.
- **Internal Logic/Purpose**: Creates an executable named "GLEngine" using the source files listed in `SOURCES`.

#### target_include_directories(GLEngine PRIVATE
    src/
    Libraries/include/
)
- **Full Signature**: `target_include_directories(GLEngine PRIVATE src/ Libraries/include/)`
- **Return Type Explanation**: This function sets include directories for a target.
- **Internal Logic/Purpose**: Specifies the directories to be included when compiling the "GLEngine" target.

#### find_package(OpenGL REQUIRED)
- **Full Signature**: `find_package(OpenGL REQUIRED)`
- **Return Type Explanation**: This function finds and loads package information.
- **Internal Logic/Purpose**: Searches for the OpenGL package, which is required for the project.

#### find_package(glfw3 REQUIRED)
- **Full Signature**: `find_package(glfw3 REQUIRED)`
- **Return Type Explanation**: This function finds and loads package information.
- **Internal Logic/Purpose**: Searches for the GLFW3 package, which is required for the project.

#### target_link_libraries(GLEngine PRIVATE
    OpenGL::GL
    glfw
    dl
)
- **Full Signature**: `target_link_libraries(GLEngine PRIVATE OpenGL::GL glfw dl)`
- **Return Type Explanation**: This function specifies libraries to link against a target.
- **Internal Logic/Purpose**: Links the "GLEngine" target against the OpenGL, GLFW, and `dl` (dynamic linking) libraries.

#### set_target_properties(GLEngine PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
- **Full Signature**: `set_target_properties(GLEngine PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")`
- **Return Type Explanation**: This function sets properties for a target.
- **Internal Logic/Purpose**: Sets the working directory for the debugger to be the source directory of the project.