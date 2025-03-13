#pragma once

// Standard library imports
#include <string>
#include <vector>

// Third-party imports
#include "handmade_math/handmade_math.h"


/**
 * @brief Struct representing a joint in a skeleton hierarchy
 */
struct Joint 
{
    // Name of the joint
    std::string name;

    // Index of parent joint (-1 for root)
    int parent_id;

    // Local transformation matrix
    HMM_Mat4 local_transform;
};

/**
 * @brief Struct representing a skeleton hierarchy
 */
struct Skeleton 
{    
    // Calculate global transforms from local transforms
    void calculate_global_transforms(std::vector<HMM_Mat4>& global_transforms);

    std::vector<Joint> joints;
};
