#include "test_framework.h"
#include "../src/MeshSkinner.h"
#include "../src/model/Mesh.h"
#include "../src/model/Skeleton.h"
#include "../src/model/SkinningData.h"
#include "test_utils.h"

TestSuite create_skinning_operations_tests() {
    TestSuite suite("Skinning Operations");
    
    suite.add_test("Full Skinning Pipeline", []() {
        MeshSkinner skinner;
        bool loaded_mesh = skinner.load_mesh("test_data/cube.obj");
        bool loaded_weights = skinner.load_weights("test_data/simple_weights.json");
        bool loaded_bind_pose = skinner.load_bind_pose("test_data/simple_bind_pose.json");
        bool loaded_new_pose = skinner.load_new_pose("test_data/simple_new_pose.json");
        
        if (!loaded_mesh || !loaded_weights || !loaded_bind_pose || !loaded_new_pose) {
            return false;
        }
        
        return skinner.perform_skinning();
    });
    
    suite.add_test("Vertex Transformation Calculation", []() {
        // Mock test environment
        Mesh original_mesh;
        original_mesh.vertices = {
            {0.0f, 0.0f, 0.0f},  // Vertex 0
            {1.0f, 0.0f, 0.0f}   // Vertex 1
        };
        
        // Two bones: one at origin, one at (0,1,0)
        Skeleton bind_pose;
        bind_pose.bone_transforms = {
            Matrix4x4::identity(),
            Matrix4x4::translation(Vector3(0.0f, 1.0f, 0.0f))
        };
        bind_pose.parent_indices = {-1, 0};
        bind_pose.calculate_global_transforms();
        
        // New pose: rotate root 90 degrees around Z
        Skeleton new_pose;
        new_pose.bone_transforms = {
            Matrix4x4::rotation_z(90.0f * (3.14159f/180.0f)),
            Matrix4x4::translation(Vector3(0.0f, 1.0f, 0.0f))
        };
        new_pose.parent_indices = {-1, 0};
        new_pose.calculate_global_transforms();
        
        // Vertex 0 influenced 100% by bone 0
        // Vertex 1 influenced 50% by bone 0, 50% by bone 1
        SkinningData skin_data;
        skin_data.weights = {
            {0, {{0, 1.0f}}},
            {1, {{0, 0.5f}, {1, 0.5f}}}
        };
        
        // Calculate skinning matrices
        std::vector<Matrix4x4> inverse_bind_matrices = bind_pose.calculate_inverse_bind_matrices();
        std::vector<Matrix4x4> skinning_matrices(bind_pose.bone_transforms.size());
        
        for (size_t i = 0; i < skinning_matrices.size(); i++) {
            skinning_matrices[i] = matrix_multiply(
                new_pose.global_transforms[i],
                inverse_bind_matrices[i]
            );
        }
        
        // Calculate transformed vertices
        Mesh skinned_mesh = original_mesh;
        
        // Transform vertex 0 (100% bone 0)
        Vector3 new_pos0 = transform_point(skinning_matrices[0], original_mesh.vertices[0]);
        
        // Transform vertex 1 (50% bone 0, 50% bone 1)
        Vector3 pos1_from_bone0 = transform_point(skinning_matrices[0], original_mesh.vertices[1]);
        Vector3 pos1_from_bone1 = transform_point(skinning_matrices[1], original_mesh.vertices[1]);
        Vector3 new_pos1 = {
            pos1_from_bone0.x * 0.5f + pos1_from_bone1.x * 0.5f,
            pos1_from_bone0.y * 0.5f + pos1_from_bone1.y * 0.5f,
            pos1_from_bone0.z * 0.5f + pos1_from_bone1.z * 0.5f
        };
        
        // After 90 degree Z rotation, vertex 0 should be at (0,0,0)
        // Vertex 1 should be partly rotated toward (0,1,0)
        bool vertex0_correct = approx_equal_vec3(new_pos0, {0.0f, 0.0f, 0.0f});
        bool vertex1_correct = approx_equal_vec3(new_pos1, {0.0f, 0.5f, 0.0f});
        
        return vertex0_correct && vertex1_correct;
    });
    
    suite.add_test("Save Skinned Mesh", []() {
        MeshSkinner skinner;
        
        // Set up and perform skinning
        if (!skinner.load_mesh("test_data/cube.obj") ||
            !skinner.load_weights("test_data/simple_weights.json") ||
            !skinner.load_bind_pose("test_data/simple_bind_pose.json") ||
            !skinner.load_new_pose("test_data/simple_new_pose.json") ||
            !skinner.perform_skinning()) {
            return false;
        }
        
        // Save result
        return skinner.save_skinned_mesh("test_data/skinned_output.obj");
    });
    
    return suite;
}
