# MeshSkinner

![MeshSkinner Logo](https://img.shields.io/badge/MeshSkinner-3D%20Animation%20Tool-blue)

A high-performance C++ library for linear blend skinning of 3D meshes. MeshSkinner takes a static 3D mesh and bone animation data to produce animated, deformed meshes.

## 🎯 Features

- **Linear Blend Skinning**: Apply skeletal animation to static meshes
- **OBJ File Support**: Load and save industry-standard OBJ files
- **JSON Configuration**: Define weights and transformations using easy-to-edit JSON
- **Parallel Processing**: Optimized with parallel algorithms for fast performance
- **Comprehensive Testing**: Robust test suite ensures reliability
- **Modular Architecture**: Clean separation of concerns via facade pattern

## 📋 Table of Contents

- [Installation](#-installation)
- [Usage](#-usage)
- [Project Structure](#-project-structure)
- [Technical Architecture](#-technical-architecture)
- [Development Setup](#-development-setup)
- [Running Tests](#-running-tests)
- [Dependencies](#-dependencies)
- [License](#-license)

## 🔧 Installation

### Prerequisites

- CMake (3.10 or higher)
- C++17 compatible compiler
- Ninja build system (recommended)

### Building from Source

```bash
# Clone the repository
git clone https://github.com/damianiRiccardo90/MeshSkinner.git
cd MeshSkinner

# Configure with CMake
cmake -B build -S . -G Ninja

# Build
cmake --build build
```

## 🚀 Usage

### Command Line

```bash
./MeshSkinner <input_mesh.obj> <bone_weight.json> <inverse_bind_pose.json> <output_pose.json> <output_mesh.obj>
```

### Example

```bash
./MeshSkinner asset/input_mesh.obj asset/bone_weights.json asset/inverse_bind_pose.json asset/output_pose.json asset/output_mesh.obj
```

### Required Input Files

1. **input_mesh.obj** - The static mesh to be skinned (OBJ format)
2. **bone_weight.json** - Vertex-to-bone weight mapping
3. **inverse_bind_pose.json** - Inverse bind pose matrices for each bone
4. **output_pose.json** - Target pose matrices for animation

### JSON Format Examples

#### bone_weights.json
```json
[
  {
    "index": [0, 1, 2, 3],
    "weight": [0.7, 0.2, 0.1, 0.0]
  },
  {
    "index": [0, 1, 0, 0],
    "weight": [0.8, 0.2, 0.0, 0.0]
  }
]
```

#### matrix files (inverse_bind_pose.json & output_pose.json)
```json
[
  [1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0],
  [1.0, 0.0, 0.0, 0.0, 0.0, 0.866, 0.5, 0.0, 0.0, -0.5, 0.866, 0.0, 0.0, 0.0, 0.0, 1.0]
]
```

## 📁 Project Structure

```
MeshSkinner/
├── src/                    # Source code
│   ├── facade/             # Facade pattern implementations
│   │   ├── json_facade.*   # JSON handling abstraction
│   │   ├── math_facade.*   # Math operations abstraction
│   │   └── obj_facade.*    # OBJ file handling abstraction
│   ├── model/              # Data structures
│   │   ├── mesh.*          # 3D mesh representation
│   │   └── skinning_data.* # Skinning data structures
│   ├── test/               # Test framework and test cases
│   ├── main.cpp            # Main application entry point
│   └── mesh_skinner.*      # Core skinning implementation
├── asset/                  # Sample assets and test data
├── build/                  # Build output (generated)
├── .vscode/                # VSCode configuration
│   ├── launch.json         # Debug configurations
│   └── tasks.json          # Build tasks
└── CMakeLists.txt          # CMake build configuration
```

## 🔍 Technical Architecture

MeshSkinner follows a modular architecture with clear separation of concerns:

### Core Components

- **MeshSkinner**: Main class that orchestrates the skinning process
- **Mesh**: Represents 3D mesh with vertices and faces
- **SkinningData**: Contains bone weights and transformation matrices

### Facade Pattern

The project uses the facade pattern to simplify interactions with complex subsystems:

- **ObjFacade**: Simplifies interactions with the tinyobjloader library
- **JsonFacade**: Provides a clean interface to nlohmann/json
- **MathFacade**: Abstracts HandmadeMath operations

### Workflow

1. Load mesh from OBJ file
2. Load weights and transforms from JSON files
3. Apply skinning algorithm (linear blend skinning)
4. Output the deformed mesh as OBJ

## 🛠️ Development Setup

### VSCode Configuration

The repository includes VSCode configurations for building and debugging:

1. Open the project in VSCode
2. Use the included tasks.json for building:
   - `Configure (CMake)`: Set up the build environment
   - `Build (CMake)`: Build the main application
   - `Build Tests (CMake)`: Build the test suite

3. Use launch.json for debugging:
   - `Run with Cygwin (CMake)`: Run and debug the main application
   - `Run Tests (CMake)`: Run and debug the test suite

## 🧪 Running Tests

Run comprehensive tests to ensure everything is working correctly:

```bash
# Build tests
cmake --build build --target MeshSkinnerTests

# Run tests
./build/MeshSkinnerTests
```

The test suite validates:
- Mesh loading and saving
- Skinning data parsing
- Transformation calculations
- End-to-end skinning process

## 📚 Dependencies

- **HandmadeMath**: Header-only math library for 3D operations
- **nlohmann/json**: Modern JSON parser for C++
- **tinyobjloader**: Tiny but powerful OBJ file loader

## 📄 License

Copyright © 2023 Riccardo Damiani

This project is licensed under the MIT License - see the LICENSE file for details.
