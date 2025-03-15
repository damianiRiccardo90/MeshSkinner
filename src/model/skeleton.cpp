#include "Skeleton.h"

// Third-party imports
#include "handmade_math/handmade_math.h"

// Local application imports
#include "facade/json_facade.h"
#include "facade/math_facade.h"


Skeleton Skeleton::from_json(const Json& json_obj)
{
    // Validate that "joints" key exists and is an array
    if (!json_obj.contains("joints"))
    {
        throw std::runtime_error("Invalid skeleton JSON: missing 'joints'");
    }

    // Retrieve the "joints" array
    const Json& joints_json = json_obj["joints"];
    if (joints_json.size() == 0)
    {
        throw std::runtime_error("Invalid skeleton JSON: 'joints' array is empty");
    }

    Skeleton skeleton;
    skeleton.joints.clear();

    // Parse each joint
    for (size_t i = 0; i < joints_json.size(); ++i)
    {
        const Json& joint_obj = joints_json.at(i);

        // Check required fields
        if (!joint_obj.contains("name") ||
            !joint_obj.contains("parent_id") ||
            !joint_obj.contains("local_transform"))
        {
            throw std::runtime_error("Skeleton joint is missing required fields");
        }

        Joint j;
        j.name = joint_obj["name"].as_string();
        j.parent_id = joint_obj["parent_id"].as_int();

        // local_transform should be a 16-element array
        const Json& transform_array = joint_obj["local_transform"];
        if (transform_array.size() != 16)
        {
            throw std::runtime_error("local_transform must have 16 float entries");
        }

        for (int row = 0; row < 4; row++)
        {
            for (int col = 0; col < 4; col++)
            {
                j.local_transform[row][col] = transform_array.at(row * 4 + col).as_float();
            }
        }

        skeleton.joints.push_back(j);
    }

    return skeleton;
}

void Skeleton::calculate_global_transforms(std::vector<HMM_Mat4>& global_transforms) 
{
    // Resize the output vector to match the number of joints
    global_transforms.resize(joints.size());
    
    // Calculate global transforms in hierarchy order
    for (size_t i = 0; i < joints.size(); i++) 
    {
        if (joints[i].parent_id == -1) 
        {
            // Root joint - global transform equals local transform
            global_transforms[i] = joints[i].local_transform;
        } 
        else 
        {
            // Child joint - global transform is parent's global transform 
            // multiplied by joint's local transform
            global_transforms[i] = MathFacade::multiply(
                global_transforms[joints[i].parent_id], 
                joints[i].local_transform
            );
        }
    }
}
