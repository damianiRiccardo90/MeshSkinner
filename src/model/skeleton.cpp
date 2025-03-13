#include "Skeleton.h"

// Local application imports
#include "facade/MathFacade.h"


void Skeleton::calculate_global_transforms(std::vector<HMM_Mat4>& global_transforms) 
{
    global_transforms.resize(joints.size());
    
    // Calculate global transforms in hierarchy order
    for (size_t i = 0; i < joints.size(); i++) 
    {
        if (joints[i].parent_id == -1) 
        {
            // Root joint - global = local
            global_transforms[i] = joints[i].local_transform;
        } 
        else 
        {
            // Child joint - global = parent_global * local
            global_transforms[i] = MathFacade::multiply(
                global_transforms[joints[i].parent_id], 
                joints[i].local_transform
            );
        }
    }
}
