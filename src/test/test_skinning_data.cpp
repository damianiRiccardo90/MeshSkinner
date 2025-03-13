#include "test_framework.h"
#include "../src/model/SkinningData.h"
#include "../src/model/Mesh.h"
#include "test_utils.h"
#include <vector>

TestSuite create_skinning_data_tests() {
    TestSuite suite("Skinning Data");
    
    suite.add_test("Load Valid Weights File", []() {
        SkinningData data;
        return data.load_from_file("test_data/simple_weights.json");
    });
    
    suite.add_test("Handle Invalid Weights File", []() {
        SkinningData data;
        return !data.load_from_file("test_data/nonexistent_weights.json");
    });
    
    suite.add_test("Validate Weight Normalization", []() {
        // Create test data with unnormalized weights
        SkinningData data;
        data.weights = {
            {0, {{0, 0.5f}, {1, 0.7f}}},  // Sum = 1.2, needs normalization
            {1, {{0, 0.2f}, {1, 0.3f}}},  // Sum = 0.5, needs normalization
        };
        
        data.normalize_weights();
        
        // Check first vertex weights sum to ~1.0
        float sum1 = 0.0f;
        for (const auto& influence : data.weights[0]) {
            sum1 += influence.second;
        }
        
        // Check second vertex weights sum to ~1.0
        float sum2 = 0.0f;
        for (const auto& influence : data.weights[1]) {
            sum2 += influence.second;
        }
        
        return approx_equal(sum1, 1.0f) && approx_equal(sum2, 1.0f);
    });
    
    suite.add_test("Apply Weight Threshold", []() {
        SkinningData data;
        data.weights = {
            {0, {{0, 0.7f}, {1, 0.2f}, {2, 0.05f}, {3, 0.05f}}},
        };
        
        // Apply threshold that should remove bones 2 and 3
        data.apply_threshold(0.1f);
        
        // Should only have two influences left
        bool correct_count = data.weights[0].size() == 2;
        
        // Should only contain bones 0 and 1
        bool has_bone0 = false;
        bool has_bone1 = false;
        bool has_other_bones = false;
        
        for (const auto& influence : data.weights[0]) {
            if (influence.first == 0) has_bone0 = true;
            else if (influence.first == 1) has_bone1 = true;
            else has_other_bones = true;
        }
        
        return correct_count && has_bone0 && has_bone1 && !has_other_bones;
    });
    
    return suite;
}
