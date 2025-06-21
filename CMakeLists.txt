cmake_minimum_required(VERSION 3.16)
project(BreakoutGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(include)
include_directories(${OPENGL_INCLUDE_DIRS})

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries
target_link_libraries(${PROJECT_NAME} 
    ${OPENGL_LIBRARIES}
    glfw
    GLEW::GLEW
)

# Copy resources to build directory
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR})