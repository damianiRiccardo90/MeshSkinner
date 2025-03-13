cmake_minimum_required(VERSION 3.10)
project(MeshSkinner)

# Include third-party libraries
include_directories(third_party/handmade_math)
include_directories(third_party/nlohmann)
include_directories(third_party/tiny_obj_loader)

# Create a library of the core functionality for both main app and tests

add_library(MeshSkinnerLib STATIC
    src/facade/json_facade.cpp
    src/facade/json_facade.h
    src/facade/math_facade.cpp
    src/facade/math_facade.h
    src/facade/obj_facade.cpp
    src/facade/obj_facade.h
    src/model/mesh.h
    src/model/skeleton.cpp
    src/model/skeleton.h
    src/model/skinning_data.h
    src/mesh_skinner.cpp
    src/mesh_skinner.h
    # Add any other source files that should be shared with tests
)

add_library(MeshSkinnerTestsLib STATIC
    src/test/test_framework.cpp
    src/test/test_framework.h
    src/test/test_mesh_loading.cpp
    src/test/test_skeleton.cpp
    src/test/test_skinning_data.cpp
    src/test/test_skinning_operations.cpp
    src/test/test_utils.cpp
    src/test/test_utils.h
)

# Add main executable
add_executable(MeshSkinner src/main.cpp)
target_link_libraries(MeshSkinner MeshSkinnerLib)

# Add test executable
add_executable(MeshSkinnerTests src/test/test_main.cpp)
target_link_libraries(MeshSkinnerTests MeshSkinnerLib MeshSkinnerTestsLib)