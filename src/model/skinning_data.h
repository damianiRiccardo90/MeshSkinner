#pragma once

// Standard library imports
#include <vector>

// Third-party imports
#include "handmade_math/handmade_math.h"


/**
 * Represents the influence of a single joint on a vertex
 */
struct JointInfluence 
{
    // ID of the joint in the skeleton
    int joint_id;

    // Weight of influence (between .0f and 1.f)
    float weight;
};

/**
 * Struct containing skinning data (weights and matrices)
 */
struct SkinningData 
{
    // Weights for each vertex (outer vector = vertices, inner vector = joint influences)
    std::vector<std::vector<JointInfluence>> weights;
    
    // Skinning matrices calculated during the skinning process
    std::vector<HMM_Mat4> skinning_matrices;
};
