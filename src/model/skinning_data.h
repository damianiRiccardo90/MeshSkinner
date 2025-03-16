#pragma once

// Standard library imports
#include <vector>

// Third-party imports
#include "handmade_math/handmade_math.h"


class Json;

/**
 * @brief Represents the weights and joint influences for a single vertex
 *
 * This structure stores a fixed number of joint influences for each vertex,
 * optimized for cache efficiency and memory usage. Each vertex has up to
 * MAX_INFLUENCES joints affecting its position.
 */
struct VertexWeights 
{
    /**
     * @brief Maximum number of joint influences per vertex
     */
    static constexpr size_t MAX_INFLUENCES = 4;
    
    /**
     * @brief IDs of the joints influencing this vertex
     *
     * Array of joint indices, corresponding by position to the weights array.
     * Unused influences should have a joint_id of 0 and weight of 0.
     */
    int joint_ids[MAX_INFLUENCES];
    
    /**
     * @brief Weights of each joint's influence on this vertex
     *
     * Array of weight values between 0 and 1, with the sum typically equaling 1.0.
     * Higher values indicate stronger influence on the final vertex position.
     */
    float weights[MAX_INFLUENCES];
};

/**
 * @brief Contains all data needed for mesh skinning operations
 *
 * This structure holds the vertex-joint influence mappings and the transformation
 * matrices used during the skinning process. It provides the necessary data to
 * deform a mesh according to pose matrices and inverse bind pose matrices.
 */
struct SkinningData 
{
    /**
     * @brief Parses bone weights from a Json object.
     * @param json_obj The source Json containing weight and index arrays for each vertex.
     * @return A vector of VertexWeights for each vertex.
     * @throws std::runtime_error if required fields are missing or invalid.
     */
    static std::vector<VertexWeights> parse_weights_from_json(const Json& json_obj);
    
    /**
     * @brief Parses transformation matrices from a Json array.
     * @param json_obj The source Json array of matrices (each with 16 elements).
     * @return A vector of HMM_Mat4 matrices.
     * @throws std::runtime_error if matrices are malformed.
     */
    static std::vector<HMM_Mat4> parse_matrices_from_json(const Json& json_obj);
    
    /**
     * @brief Weights for each vertex 
     *
     * Each element corresponds to a vertex in the mesh and contains
     * the joint IDs and weights that influence that vertex.
     */
    std::vector<VertexWeights> weights;
    
    /**
     * @brief Inverse bind pose matrices for each joint
     * 
     * Each matrix represents the inverse of a joint's transformation in the bind pose.
     * Used to calculate the difference between bind pose and current pose.
     */
    std::vector<HMM_Mat4> inverse_bind_matrices;
    
    /**
     * @brief Current pose matrices for each joint
     *
     * Each matrix represents the transformation of a joint in the current pose.
     * Combined with inverse bind matrices to create skinning matrices.
     */
    std::vector<HMM_Mat4> pose_matrices;
    
    /**
     * @brief Calculates and returns the skinning matrix for a specific joint
     * 
     * @param joint_id The joint ID to get the skinning matrix for
     * @return The skinning matrix (pose_matrix * inverse_bind_matrix)
     * @throws std::out_of_range if joint_id is invalid
     */
    HMM_Mat4 get_skinning_matrix(int joint_id) const;
};
