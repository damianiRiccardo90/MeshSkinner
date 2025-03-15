#pragma once

// Standard library imports
#include <string>
#include <vector>

// Third-party imports
#include "handmade_math/handmade_math.h"


struct Json;

/**
 * @brief Struct representing a joint in a skeleton hierarchy
 *
 * A joint is a single point in the skeleton structure that can be animated.
 * Each joint maintains its relationship to a parent joint and stores its local
 * transformation matrix.
 */
struct Joint 
{
    /**
     * @brief Name identifier for the joint
     *
     * Used for debugging and for mapping joints to animation data.
     */
    std::string name;

    /**
     * @brief Index of parent joint in the joints array
     *
     * A value of -1 indicates this is a root joint with no parent.
     */
    int parent_id;

    /**
     * @brief Local transformation matrix relative to the parent joint
     *
     * This transformation is defined in the parent joint's coordinate space.
     */
    HMM_Mat4 local_transform;
};

/**
 * @brief Struct representing a skeleton hierarchy for character animation
 *
 * A skeleton is composed of a hierarchy of joints that define the underlying structure
 * of a 3D model. It's used for skeletal animation where vertices in a mesh are bound
 * to one or more bones with different weights.
 */
struct Skeleton 
{
    /**
     * @brief Construct a Skeleton structure by parsing a JSON object.
     * @param json_obj The JSON data containing the skeleton definition.
     * @return A Skeleton object parsed from the JSON.
     * @throws std::runtime_error if parsing fails or fields are missing/invalid.
     */
    static Skeleton from_json(const Json& json_obj);

    /**
     * @brief Calculate global transforms for all joints from their local transforms
     *
     * Global transforms represent each joint's position and orientation in the model's
     * coordinate space. This method traverses the joint hierarchy in order, computing
     * each joint's global transform by combining its local transform with its parent's
     * global transform.
     *
     * @param global_transforms Output vector to store the calculated global transformation matrices
     * @note The global_transforms vector will be resized to match the number of joints
     */
    void calculate_global_transforms(std::vector<HMM_Mat4>& global_transforms);

    /**
     * @brief Collection of joints that make up the skeleton
     *
     * Joints are stored in an array where parent joints must appear before their children.
     * This ordering ensures that a parent's global transform can be calculated before 
     * its children's transforms.
     */
    std::vector<Joint> joints;
};
