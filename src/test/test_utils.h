#pragma once

// Standard library imports
#include <vector>
#include <utility>

// Third-party imports
#include "handmade_math/handmade_math.h"


struct Face;

namespace TestUtils {

/**
 * @brief Compares two floating point values for approximate equality.
 *
 * This function determines if two float values are approximately equal within
 * a specified epsilon tolerance, accounting for floating point imprecision.
 *
 * @param a The first float value to compare.
 * @param b The second float value to compare.
 * @param epsilon The maximum allowed difference between values (default: .0001f).
 * @return true if the absolute difference between a and b is less than epsilon; otherwise false.
 */
bool approx_equal(float a, float b, float epsilon = .0001f);

/**
 * @brief Compares two 3D vectors for approximate equality.
 *
 * This function determines if two HMM_Vec3 vectors are approximately equal by
 * comparing each component (X, Y, Z) using the approx_equal function.
 *
 * @param a The first vector to compare.
 * @param b The second vector to compare.
 * @param epsilon The maximum allowed difference between component values (default: .0001f).
 * @return true if all components are approximately equal; otherwise false.
 */
bool approx_equal_vec3(const HMM_Vec3& a, const HMM_Vec3& b, float epsilon = .0001f);

/**
 * @brief Compares two 4x4 matrices for approximate equality.
 *
 * This function determines if two HMM_Mat4 matrices are approximately equal by
 * comparing each element using the approx_equal function.
 *
 * @param a The first matrix to compare.
 * @param b The second matrix to compare.
 * @param epsilon The maximum allowed difference between element values (default: .0001f).
 * @return true if all elements are approximately equal; otherwise false.
 */
bool approx_equal_mat4(const HMM_Mat4& a, const HMM_Mat4& b, float epsilon = .0001f);

/**
 * @brief Checks if a vertex is connected to any face in the mesh.
 *
 * This function determines whether a vertex at the specified index is used
 * by any face in the mesh. A vertex that isn't connected to any face is
 * considered "disconnected" or "isolated" and might indicate a problem
 * with the mesh structure.
 *
 * @param faces Vector of Face objects representing the mesh topology.
 * @param vertex_index The index of the vertex to check for connectivity.
 * @return true if the vertex is used by at least one face; otherwise false.
 */
bool is_vertex_connected(const std::vector<Face>& faces, size_t vertex_index);

/**
 * @brief Calculates the axis-aligned bounding box of a mesh.
 *
 * This function computes the minimum and maximum coordinate values across
 * all vertices in the mesh, defining a bounding box that completely
 * encloses the mesh. Useful for validation, scaling, and positioning tests.
 *
 * @param vertices Vector of Vector3 objects representing the mesh vertices.
 * @return A pair of Vector3 objects representing the minimum and maximum bounds.
 *         Returns {{0,0,0}, {0,0,0}} if the vertices vector is empty.
 */
std::pair<HMM_Vec3, HMM_Vec3> calculate_mesh_bounds(const std::vector<HMM_Vec3>& vertices);

} // namespace TestUtils
