// Standard library imports
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

// Local application imports
#include "facade/json_facade.h"
#include "model/skinning_data.h"
#include "test/test_framework.h"


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

            std::cout << "Loading skinning data: Success\n";
            std::cout << "Loaded weights for " << weights.size() << " vertices\n";

            // Check basic validity - non-empty weights
            return !weights.empty();
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Loading skinning data failed: " << e.what() << std::endl;
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

            std::cout << "Failed to reject nonexistent file\n";
            return false;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Correctly rejected nonexistent file\n";
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

            std::cout << "Failed to reject malformed JSON file\n";
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Correctly rejected malformed JSON file\n";
            
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

            std::cout << "Failed to reject JSON missing required fields\n";
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Correctly rejected JSON missing required fields\n";
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return true;
        }
    });

    return suite;
}
