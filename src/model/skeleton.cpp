#include "Skeleton.h"

// Local application imports
#include "facade/math_facade.h"


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
