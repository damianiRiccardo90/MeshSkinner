#pragma once

// Standard library imports
#include <vector>

// Third-party imports
#include "handmade_math/handmade_math.h"


class Json;

/**
 * @brief Represents the influence of a single joint on a vertex
 *
 * This structure defines how much influence a specific joint has on the 
 * deformation of a vertex during skeletal animation. Each vertex may be
 * influenced by multiple joints with different weights.
 */
struct JointInfluence 
{
    /**
     * @brief ID of the joint in the skeleton
     *
     * References a specific joint in the skeleton hierarchy by its index.
     */
    int joint_id;

    /**
     * @brief Weight of influence (between 0.f and 1.f)
     *
     * Determines how much this joint affects the vertex position.
     * Higher values indicate stronger influence on the final vertex position.
     * The sum of all weights for a single vertex should typically equal 1.0.
     */
    float weight;
};

/**
 * @brief Contains all data needed for mesh skinning operations
 *
 * This structure holds the vertex-joint influence mappings and the computed
 * transformation matrices used during the skinning process. It provides the
 * necessary data to deform a mesh according to an animated skeleton pose.
 */
struct SkinningData 
{
    /**
     * @brief Parses skinning data (weights) from a Json object.
     * @param json_obj The source Json containing "vertex_weights".
     * @return A nested vector of JointInfluence for each vertex.
     * @throws std::runtime_error if "vertex_weights" or fields are missing/invalid.
     */
    static std::vector<std::vector<JointInfluence>> from_json(const Json& json_obj);

    /**
     * @brief Weights for each vertex 
     *
     * The outer vector corresponds to vertices in the mesh.
     * The inner vector contains all joint influences for a single vertex.
     * Each vertex can be influenced by multiple joints with varying weights.
     */
    std::vector<std::vector<JointInfluence>> weights;
    
    /**
     * @brief Skinning matrices calculated during the skinning process
     *
     * Each matrix represents the transformation from bind pose to current pose
     * for a specific joint. These matrices are used to transform vertices
     * according to the current skeleton pose.
     */
    std::vector<HMM_Mat4> skinning_matrices;
};
