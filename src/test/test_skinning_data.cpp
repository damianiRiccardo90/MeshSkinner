// Standard library imports
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

// Local application imports
#include "facade/json_facade.h"
#include "model/skinning_data.h"
#include "test/test_framework.h"
#include "test/test_utils.h"


TestSuite create_skinning_data_tests() 
{
    TestSuite suite("Skinning Data");
    
    // Basic file loading test
    suite.add_test("Load Valid Skinning Data", []() 
    {
        try 
        {
            const Json json_data = JsonFacade::load_from_file("asset/bone_weights.json");

            const std::vector<std::vector<JointInfluence>> weights = 
                SkinningData::from_json(json_data);

            TestUtils::print_colored("Loading skinning data: Success\n", 
                TestUtils::ConsoleColor::Green);
            
            TestUtils::set_console_color(TestUtils::ConsoleColor::Default);
            std::cout << "Loaded weights for " << weights.size() << " vertices\n";
            TestUtils::reset_console_color();

            // Check basic validity - non-empty weights
            return !weights.empty();
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Loading skinning data failed: " << e.what() << std::endl;
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

            const std::vector<std::vector<JointInfluence>> weights = 
                SkinningData::from_json(json_data);

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
    
    // Edge case: Malformed JSON
    suite.add_test("Handle Malformed JSON", []() 
    {
        // Create temporary malformed JSON file
        const std::string tempFilePath = "asset/temp_malformed_weights.json";
        std::ofstream tempFile(tempFilePath);
        tempFile << "{ this is not valid JSON }";
        tempFile.close();

        try 
        {
            const Json json_data = JsonFacade::load_from_file(tempFilePath);

            const std::vector<std::vector<JointInfluence>> weights = 
                SkinningData::from_json(json_data);

            TestUtils::print_colored("Failed to reject malformed JSON file\n", 
                TestUtils::ConsoleColor::Red);
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::print_colored("Correctly rejected malformed JSON file\n", 
                TestUtils::ConsoleColor::Green);
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return true;
        }
    });
    
    // Edge case: Missing vertex_weights field
    suite.add_test("Handle Missing vertex_weights Field", []() 
    {
        // Create JSON without vertex_weights field
        const std::string tempFilePath = "asset/temp_missing_field.json";
        std::ofstream tempFile(tempFilePath);
        tempFile << "{ \"other_field\": [] }";
        tempFile.close();

        try 
        {
            const Json json_data = JsonFacade::load_from_file(tempFilePath);

            const std::vector<std::vector<JointInfluence>> weights = 
                SkinningData::from_json(json_data);

            TestUtils::print_colored("Failed to reject JSON missing required fields\n", 
                TestUtils::ConsoleColor::Red);
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::print_colored("Correctly rejected JSON missing required fields\n", 
                TestUtils::ConsoleColor::Green);
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return true;
        }
    });

    // Add a test for weight normalization
    suite.add_test("Weight Normalization", []() 
    {
        TestUtils::print_colored("Testing weight normalization...\n", 
            TestUtils::ConsoleColor::Default);
        
        // Create a temporary JSON file with unnormalized weights
        const std::string tempFilePath = "asset/temp_unnormalized_weights.json";
        std::ofstream tempFile(tempFilePath);
        tempFile << R"({
            "vertex_weights": [
                [
                    {"joint_id": 0, "weight": 0.7},
                    {"joint_id": 1, "weight": 0.6}
                ],
                [
                    {"joint_id": 0, "weight": 0.3},
                    {"joint_id": 1, "weight": 0.3},
                    {"joint_id": 2, "weight": 0.3}
                ]
            ]
        })";
        tempFile.close();

        try 
        {
            const Json json_data = JsonFacade::load_from_file(tempFilePath);

            const std::vector<std::vector<JointInfluence>> weights = 
                SkinningData::from_json(json_data);
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            // Check if weights are properly normalized
            bool all_normalized = true;
            
            for (size_t i = 0; i < weights.size(); ++i) 
            {
                float sum = 0.0f;
                for (const auto& influence : weights[i]) 
                {
                    sum += influence.weight;
                }
                
                const bool is_normalized = TestUtils::approx_equal(sum, 1.0f);
                
                TestUtils::set_console_color(is_normalized ? 
                    TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
                std::cout << "Vertex " << i << " weights sum: " << sum 
                          << (is_normalized ? " (correctly normalized)\n" : " (not normalized)\n");
                TestUtils::reset_console_color();
                
                all_normalized &= is_normalized;
            }
            
            if (all_normalized) 
            {
                TestUtils::print_colored("All weights are correctly normalized\n", 
                    TestUtils::ConsoleColor::Green);
            } 
            else 
            {
                TestUtils::print_colored("Some weights are not properly normalized\n", 
                    TestUtils::ConsoleColor::Red);
            }
            
            return all_normalized;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Weight normalization test failed: " << e.what() << std::endl;
            TestUtils::reset_console_color();
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return false;
        }
    });

    return suite;
}
