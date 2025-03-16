// Standard library imports
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

// Local application imports
#include "facade/json_facade.h"
#include "facade/math_facade.h"
#include "model/skinning_data.h"
#include "test/test_framework.h"
#include "test/test_utils.h"


TestSuite create_skinning_data_tests() 
{
    TestSuite suite("Skinning Data");
    
    // Test parsing weights from JSON
    suite.add_test("Parse Vertex Weights from JSON", []() 
    {
        try 
        {
            const Json json_data = JsonFacade::load_from_file("asset/bone_weights.json");

            const std::vector<VertexWeights> weights = 
                SkinningData::parse_weights_from_json(json_data);

            TestUtils::print_colored("Parsing vertex weights: Success\n", 
                TestUtils::ConsoleColor::Green);
            
            TestUtils::set_console_color(TestUtils::ConsoleColor::Default);
            std::cout << "Parsed weights for " << weights.size() << " vertices\n";
            TestUtils::reset_console_color();

            // Check basic validity - non-empty weights
            return !weights.empty();
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Parsing vertex weights failed: " << e.what() << std::endl;
            TestUtils::reset_console_color();

            return false;
        }
    });
    
    // Test parsing matrices from JSON
    suite.add_test("Parse Matrices from JSON", []() 
    {
        try 
        {
            const Json json_data = JsonFacade::load_from_file("asset/inverse_bind_pose.json");

            const std::vector<HMM_Mat4> matrices = 
                SkinningData::parse_matrices_from_json(json_data);

            TestUtils::print_colored("Parsing matrices: Success\n", 
                TestUtils::ConsoleColor::Green);
            
            TestUtils::set_console_color(TestUtils::ConsoleColor::Default);
            std::cout << "Parsed " << matrices.size() << " matrices\n";
            TestUtils::reset_console_color();

            // Check basic validity - non-empty matrices array
            return !matrices.empty();
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Parsing matrices failed: " << e.what() << std::endl;
            TestUtils::reset_console_color();

            return false;
        }
    });
    
    // Edge case: Nonexistent file
    suite.add_test("Handle Nonexistent File", []() 
    {
        try 
        {
            const Json json_data = JsonFacade::load_from_file("asset/nonexistent_weights.json");

            const std::vector<VertexWeights> weights = 
                SkinningData::parse_weights_from_json(json_data);

            TestUtils::print_colored("Failed to reject nonexistent file\n", 
                TestUtils::ConsoleColor::Red);

            return false;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::print_colored("Correctly rejected nonexistent file\n", 
                TestUtils::ConsoleColor::Green);

            return true;
        }
    });
    
    // Edge case: Malformed JSON for weights
    suite.add_test("Handle Malformed Weights JSON", []() 
    {
        // Create temporary malformed JSON file
        const std::string temp_file_path = "asset/temp_malformed_weights.json";
        std::ofstream temp_file(temp_file_path);
        temp_file << "{ this is not valid JSON }";
        temp_file.close();

        try 
        {
            const Json json_data = JsonFacade::load_from_file(temp_file_path);

            const std::vector<VertexWeights> weights = 
                SkinningData::parse_weights_from_json(json_data);

            TestUtils::print_colored("Failed to reject malformed JSON file\n", 
                TestUtils::ConsoleColor::Red);
            
            // Clean up
            std::filesystem::remove(temp_file_path);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::print_colored("Correctly rejected malformed JSON file\n", 
                TestUtils::ConsoleColor::Green);
            
            // Clean up
            std::filesystem::remove(temp_file_path);
            
            return true;
        }
    });
    
    // Edge case: Malformed matrix data
    suite.add_test("Handle Malformed Matrix JSON", []() 
    {
        // Create temporary JSON with invalid matrix data (not 16 elements)
        const std::string temp_file_path = "asset/temp_malformed_matrix.json";
        std::ofstream temp_file(temp_file_path);
        temp_file << "[[1, 2, 3]]"; // Not a valid 4x4 matrix
        temp_file.close();

        try 
        {
            const Json json_data = JsonFacade::load_from_file(temp_file_path);

            const std::vector<HMM_Mat4> matrices = 
                SkinningData::parse_matrices_from_json(json_data);

            TestUtils::print_colored("Failed to reject malformed matrix data\n", 
                TestUtils::ConsoleColor::Red);
            
            // Clean up
            std::filesystem::remove(temp_file_path);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::print_colored("Correctly rejected malformed matrix data\n", 
                TestUtils::ConsoleColor::Green);
            
            // Clean up
            std::filesystem::remove(temp_file_path);
            
            return true;
        }
    });
    
    // Test out of range joint IDs
    suite.add_test("Handle Out-of-Range Joint ID", []() 
    {
        SkinningData skin_data;
        
        // Add one matrix to the vectors
        const HMM_Mat4 identity = MathFacade::create_identity();
        
        skin_data.inverse_bind_matrices.push_back(identity);
        skin_data.pose_matrices.push_back(identity);
        
        try 
        {
            // Try to access joint ID outside range
            skin_data.get_skinning_matrix(1); // Should throw
            
            TestUtils::print_colored("Failed to reject out-of-range joint ID\n", 
                TestUtils::ConsoleColor::Red);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::print_colored("Correctly rejected out-of-range joint ID\n", 
                TestUtils::ConsoleColor::Green);
            
            return true;
        }
    });
    
    // Test joint weights validation
    suite.add_test("Joint Weights Validation", []() 
    {
        // Create a test JSON with vertex weights data
        const std::string temp_file_path = "asset/temp_test_weights.json";
        std::ofstream temp_file(temp_file_path);
        temp_file << R"([
            {
                "weight": [0.5, 0.3, 0.2, 0.0],
                "index": [0, 1, 2, 0]
            },
            {
                "weight": [1.0, 0.0, 0.0, 0.0],
                "index": [1, 0, 0, 0]
            }
        ])";
        temp_file.close();

        try 
        {
            const Json json_data = JsonFacade::load_from_file(temp_file_path);
            
            const std::vector<VertexWeights> weights = 
                SkinningData::parse_weights_from_json(json_data);
            
            // Verify the first vertex has 3 non-zero weights
            int non_zero_weights1 = 0;
            for (int i = 0; i < VertexWeights::MAX_INFLUENCES; i++) 
            {
                if (weights[0].weights[i] > 0.f) 
                {
                    non_zero_weights1++;
                }
            }
            
            // Verify the second vertex has 1 weight of 1.0
            const bool second_vertex_correct = 
                TestUtils::approx_equal(weights[1].weights[0], 1.f) &&
                TestUtils::approx_equal(weights[1].weights[1], 0.f) &&
                TestUtils::approx_equal(weights[1].weights[2], 0.f) &&
                TestUtils::approx_equal(weights[1].weights[3], 0.f);
            
            const bool all_correct = (non_zero_weights1 == 3) && second_vertex_correct;
            
            TestUtils::set_console_color(all_correct ? 
                TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
            std::cout << "Weights validation test: " 
                      << (all_correct ? "Passed" : "Failed") << std::endl;
            if (!all_correct) 
            {
                std::cout << "First vertex non-zero weights: " << non_zero_weights1 
                          << " (expected 3)" << std::endl;
                std::cout << "Second vertex weight[0]: " << weights[1].weights[0] 
                          << " (expected 1.0)" << std::endl;
            }
            TestUtils::reset_console_color();
            
            // Clean up
            std::filesystem::remove(temp_file_path);
            
            return all_correct;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Weights validation test failed: " << e.what() << std::endl;
            TestUtils::reset_console_color();
            
            // Clean up
            std::filesystem::remove(temp_file_path);
            
            return false;
        }
    });

    // Test calculating skinning matrix
    suite.add_test("Calculate Skinning Matrix", []() 
    {
        try 
        {
            // Create a test SkinningData object
            SkinningData skin_data;
            
            // Create test matrices (identity matrices for simplicity)
            const HMM_Mat4 identity = MathFacade::create_identity();
            
            // Add a test inverse bind matrix
            skin_data.inverse_bind_matrices.push_back(identity);
            
            // Add a test pose matrix
            skin_data.pose_matrices.push_back(identity);
            
            // Calculate skinning matrix
            const HMM_Mat4 skinning_matrix = skin_data.get_skinning_matrix(0);
            
            // Should be identity * identity = identity
            bool is_identity = true;
            for (int i = 0; i < 4; i++) 
            {
                for (int j = 0; j < 4; j++) 
                {
                    if (i == j) 
                    {
                        // Diagonal should be 1
                        if (!TestUtils::approx_equal(skinning_matrix.Elements[i][j], 1.f)) 
                        {
                            is_identity = false;
                        }
                    } 
                    else 
                    {
                        // Off-diagonal should be 0
                        if (!TestUtils::approx_equal(skinning_matrix.Elements[i][j], 0.f)) 
                        {
                            is_identity = false;
                        }
                    }
                }
            }
            
            TestUtils::set_console_color(is_identity ? 
                TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
            std::cout << "Skinning matrix calculation: " 
                     << (is_identity ? "Correct" : "Incorrect") << std::endl;
            TestUtils::reset_console_color();
            
            return is_identity;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Skinning matrix calculation failed: " << e.what() << std::endl;
            TestUtils::reset_console_color();

            return false;
        }
    });

    // Test with realistic (non-identity) transformation matrices
    suite.add_test("Realistic Transformation Matrices", []() 
    {
        // Create a SkinningData object
        SkinningData skin_data;
        
        // Create a rotation matrix (45 degrees around Y axis)
        const float angle = MathFacade::to_radians(45.f);
        HMM_Mat4 rotation_matrix = MathFacade::rotateY(angle);
        
        // Create a translation matrix (move 1,2,3)
        HMM_Mat4 translation_matrix = MathFacade::translate(1.f, 2.f, 3.f);
        
        // Add these as inverse_bind and pose matrices
        skin_data.inverse_bind_matrices.push_back(rotation_matrix);
        skin_data.pose_matrices.push_back(translation_matrix);
        
        // Calculate skinning matrix
        HMM_Mat4 skinning_matrix = skin_data.get_skinning_matrix(0);
        
        // Test a specific vertex transformation
        const HMM_Vec3 test_vertex = HMM_V3(1.f, 1.f, 1.f);
        
        // Expected result after applying rotation then translation
        HMM_Vec3 rotated = MathFacade::transform_vec3(rotation_matrix, test_vertex);
        HMM_Vec3 expected_pos = MathFacade::transform_vec3(translation_matrix, rotated);
        
        // Transform using skinning matrix
        const HMM_Vec3 actual_pos = MathFacade::transform_vec3(skinning_matrix, test_vertex);
        
        // Compare with a reasonable epsilon
        const bool transform_correct = 
            TestUtils::approx_equal(actual_pos.X, expected_pos.X, .001f) &&
            TestUtils::approx_equal(actual_pos.Y, expected_pos.Y, .001f) &&
            TestUtils::approx_equal(actual_pos.Z, expected_pos.Z, .001f);
        
        TestUtils::set_console_color(transform_correct ? 
            TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
        std::cout << "Realistic matrix transformation test: " 
                << (transform_correct ? "Passed" : "Failed") << std::endl;
        
        if (!transform_correct) 
        {
            std::cout << "Expected: (" << expected_pos.X << ", " 
                    << expected_pos.Y << ", " << expected_pos.Z << ")" << std::endl;
            std::cout << "Actual: (" << actual_pos.X << ", " 
                    << actual_pos.Y << ", " << actual_pos.Z << ")" << std::endl;
        }
        TestUtils::reset_console_color();
        
        return transform_correct;
    });

    return suite;
}
