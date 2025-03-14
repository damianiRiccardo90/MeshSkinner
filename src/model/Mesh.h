#pragma once

// Standard library imports
#include <vector>


/**
 * @brief A simple struct representing a vertex in 3D space.
 *
 * This struct contains only the position data for a vertex, stored as three
 * floating-point coordinates in a Cartesian coordinate system.
 */
struct Vertex 
{
    /**
     * @brief The x-coordinate in 3D space.
     */
    float x;
    
    /**
     * @brief The y-coordinate in 3D space.
     */
    float y;
    
    /**
     * @brief The z-coordinate in 3D space.
     */
    float z;
};

/**
 * @brief A representation of a 3D mesh with vertices and triangle indices.
 *
 * This struct contains the fundamental data needed to represent a 3D mesh:
 * a collection of vertices and indices that define how these vertices are 
 * connected to form triangles. The indices are stored as triplets, where
 * each consecutive set of three indices defines one triangle face.
 */
struct Mesh 
{
    /**
     * @brief The collection of vertices that make up the mesh.
     */
    std::vector<Vertex> vertices;
    
    /**
     * @brief The indices that define how vertices connect to form triangles.
     *
     * Each triplet of consecutive indices (i, i+1, i+2) defines a triangle face.
     * The values reference positions in the vertices array.
     */
    std::vector<unsigned int> indices;
};
