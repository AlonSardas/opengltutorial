cmake_minimum_required(VERSION 3.15)
project(OpenGLTutorial)

# Use C++17
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_NAME main.app)

include_directories(src include)

file(GLOB_RECURSE SRC_FILES
    CONFIGURE_DEPENDS
    src/*.cpp
    src/*.c
)

set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address,undefined -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -Wall")

add_executable(${EXECUTABLE_NAME} ${SRC_FILES})

# link_directories(
#     ${HOME}/Documents/Dev/OpenGLTutorial/glfw-3.4/build/src
#     ${HOME}/Documents/Dev/OpenGLTutorial/assimp/bin
# )
# link_directories(
#     /home/alon/Documents/Dev/OpenGLTutorial/glfw-3.4/build/src
#     /home/alon/Documents/Dev/OpenGLTutorial/assimp/bin
# )
target_link_directories(${EXECUTABLE_NAME} PRIVATE
    /home/alon/Documents/Dev/OpenGLTutorial/glfw-3.4/build/src
    /home/alon/Documents/Dev/OpenGLTutorial/assimp/bin
)

target_link_libraries(${EXECUTABLE_NAME}
    GL
    glfw
    assimp
)

# Optional: Put compiled binary in the root dir like Makefile did
set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)