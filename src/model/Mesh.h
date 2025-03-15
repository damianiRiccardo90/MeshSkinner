#pragma once

// Standard library imports
#include <array>
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
 * @brief A struct representing a triangular face in a mesh.
 *
 * This struct stores the three vertex indices that make up a triangular face.
 * Each index references a position in the vertices array of the mesh.
 */
struct Face
{
    /**
     * @brief The indices of the three vertices that form this triangular face.
     * 
     * Each value references a position in the vertices array of the mesh.
     */
    std::array<unsigned int, 3> indices;
};

/**
 * @brief A representation of a 3D mesh with vertices and faces.
 *
 * This struct contains the fundamental data needed to represent a 3D mesh:
 * a collection of vertices and faces that define how these vertices are 
 * connected to form triangles.
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
    
    /**
     * @brief The collection of triangular faces that make up the mesh.
     * 
     * Each face defines a triangle by referencing three vertices in the vertices array.
     * This provides a more structured representation of the mesh topology compared to
     * the raw indices array.
     */
    std::vector<Face> faces;
};
