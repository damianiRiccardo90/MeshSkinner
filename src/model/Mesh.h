#pragma once

// Standard library imports
#include <vector>


/**
 * @brief A simple struct for a single vertex (only position here).
 */
struct Vertex 
{
    float x, y, z;
};

/**
 * @brief A minimal representation of a mesh with vertices and indices.
 */
struct Mesh 
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};
