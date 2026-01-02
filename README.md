# SFML OBJ Viewer (C++)

A lightweight C++ project that loads and displays `.obj` meshes using **SFML** for 2D drawing, with a simple custom 3D pipeline (projection + basic face sorting). Built as a learning/experimental graphics project.

## Features
- Load `.obj` models into a `Mesh / Vertex / Face` structure
- Supports faces built from **triangles and quads**
- Basic **perspective projection** to 2D
- Simple **depth-based face sorting** (painter’s algorithm style)
- Basic **movement/rotation controls** (keyboard)

## Tech Stack
- **C++**
- **SFML (Graphics)**

## Requirements
- A C++ compiler (C++17 recommended)
- SFML installed (Graphics module)

## Build & Run

### Option 1: CMake (recommended)
> If you already have a `CMakeLists.txt`, use this flow:

```bash
mkdir build
cd build
cmake ..
cmake --build .
