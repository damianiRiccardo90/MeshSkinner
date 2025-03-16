cmake_minimum_required(VERSION 3.10)
project(MeshSkinner VERSION 0.1.0 LANGUAGES CXX)

# Set C++ standard to 17 (as I'm using std::filesystem and parallel algorithms)
set(CMAKE_CXX_STANDARD 17)

# Generate symbols for debugging
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# Include project's source and third-party libraries
include_directories(
    ${PROJECT_SOURCE_DIR}/third_party
    ${PROJECT_SOURCE_DIR}/src
)

# Create libraries for both main app and tests

add_library(MeshSkinnerLib STATIC
    src/facade/json_facade.cpp
    src/facade/math_facade.cpp
    src/facade/obj_facade.cpp
    src/model/skinning_data.cpp
    src/mesh_skinner.cpp
)

add_library(MeshSkinnerTestsLib STATIC
    src/test/test_framework.cpp
    src/test/test_mesh.cpp
    src/test/test_skinner.cpp
    src/test/test_skinning_data.cpp
    src/test/test_utils.cpp
)

# Add main executable
add_executable(MeshSkinner src/main.cpp)
target_link_libraries(MeshSkinner PRIVATE MeshSkinnerLib)

# Add test executable
add_executable(MeshSkinnerTests src/test/test_main.cpp)
target_link_libraries(MeshSkinnerTests PRIVATE 
    MeshSkinnerTestsLib
    MeshSkinnerLib
)