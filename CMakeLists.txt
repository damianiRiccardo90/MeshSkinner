cmake_minimum_required(VERSION 3.10)
project(MeshSkinner)

# Include third-party libraries
include_directories(third_party/handmade_math)
include_directories(third_party/nlohmann)
include_directories(third_party/tiny_obj_loader)

# Add executable
add_executable(MeshSkinner src/main.cpp src/MeshSkinner.cpp src/MeshSkinner.h)