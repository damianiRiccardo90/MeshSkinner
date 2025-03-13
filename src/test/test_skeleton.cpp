// Standard library imports
#include <vector>

// Local application imports
#include "model/Skeleton.h"
#include "test/test_framework.h"
#include "test/test_utils.h"

// Third-party imports
#include "handmade_math/handmade_math.h"


TestSuite create_skeleton_tests() 
{
    TestSuite suite("Skeleton");
    
    suite.add_test("Load Valid Bind Pose", []() {
        Skeleton skeleton;
        return skeleton.load_from_file("test_data/simple_bind_pose.json");
    });
    
    suite.add_test("Load Valid New Pose", []() {
        Skeleton skeleton;
        return skeleton.load_from_file("test_data/simple_new_pose.json");
    });
    
    suite.add_test("Handle Invalid Pose File", []() {
        Skeleton skeleton;
        return !skeleton.load_from_file("test_data/nonexistent_pose.json");
    });
    
    suite.add_test("Bone Transformation Matrix Calculation", []() {
        Skeleton skeleton;
        
        // Create a simple test skeleton
        skeleton.bone_transforms = {
            HMM_Mat4d(1.0f),  // Root bone - identity matrix
            HMM_Translate(HMM_Vec3(1.0f, 0.0f, 0.0f))  // Child bone
        };
        
        skeleton.parent_indices = {-1, 0};  // Root has no parent, bone 1 has parent 0
        
        // Calculate global transforms
        skeleton.calculate_global_transforms();
        
        // Root should still be identity
        bool root_correct = matrix_approx_equal(
            skeleton.global_transforms[0], 
            HMM_Mat4d(1.0f)
        );
        
        // Child should have parent's transform * local transform
        bool child_correct = matrix_approx_equal(
            skeleton.global_transforms[1],
            HMM_Translate(HMM_Vec3(1.0f, 0.0f, 0.0f))
        );
        
        return root_correct && child_correct;
    });
    
    suite.add_test("Inverse Bind Pose Calculation", []() {
        Skeleton bind_pose;
        
        // Set up a simple bind pose
        bind_pose.bone_transforms = {
            HMM_Mat4d(1.0f),  // identity matrix
            HMM_Translate(HMM_Vec3(0.0f, 1.0f, 0.0f))
        };
        bind_pose.parent_indices = {-1, 0};
        bind_pose.calculate_global_transforms();
        
        // Calculate inverse bind pose
        std::vector<hmm_mat4> inverse_bind_matrices = bind_pose.calculate_inverse_bind_matrices();
        
        // Check that inverse_bind * global_transform = identity
        hmm_mat4 result1 = HMM_MultiplyMat4(
            inverse_bind_matrices[0],
            bind_pose.global_transforms[0]
        );
        
        hmm_mat4 result2 = HMM_MultiplyMat4(
            inverse_bind_matrices[1],
            bind_pose.global_transforms[1]
        );
        
        return matrix_approx_equal(result1, HMM_Mat4d(1.0f)) && 
               matrix_approx_equal(result2, HMM_Mat4d(1.0f));
    });
    
    return suite;
}
