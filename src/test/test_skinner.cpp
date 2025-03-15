// Standard library imports
#include <filesystem>
#include <iostream>

// Local application imports
#include "facade/obj_facade.h"
#include "mesh_skinner.h"
#include "model/mesh.h"
#include "test/test_framework.h"


TestSuite create_skinner_tests() 
{
    TestSuite suite("Skinning Operations");
    
    // Basic file loading tests
    suite.add_test("Load Valid Mesh", []() 
    {
        try 
        {
            MeshSkinner skinner;

            const bool success = skinner.load_mesh("asset/input_mesh.obj");
            
            std::cout << "Loading mesh: " << (success ? "Success" : "Failed") << std::endl;
            return success;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Loading mesh failed with exception: " << e.what() << std::endl;
            return false;
        }
    });
    
    suite.add_test("Load Valid Weights", []() 
    {
        try 
        {
            MeshSkinner skinner;

            const bool success = skinner.load_weights("asset/bone_weights.json");
            
            std::cout << "Loading weights: " << (success ? "Success" : "Failed") << std::endl;
            return success;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Loading weights failed with exception: " << e.what() << std::endl;
            return false;
        }
    });
    
    suite.add_test("Load Valid Bind Pose", []() 
    {
        try 
        {
            MeshSkinner skinner;

            const bool success = skinner.load_bind_pose("asset/inverse_bind_pose.json");
            
            std::cout << "Loading bind pose: " << (success ? "Success" : "Failed") << std::endl;
            return success;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Loading bind pose failed with exception: " << e.what() << std::endl;
            return false;
        }
    });
    
    suite.add_test("Load Valid New Pose", []() 
    {
        try 
        {
            MeshSkinner skinner;

            const bool success = skinner.load_new_pose("asset/output_pose.json");
            
            std::cout << "Loading new pose: " << (success ? "Success" : "Failed") << std::endl;
            return success;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Loading new pose failed with exception: " << e.what() << std::endl;
            return false;
        }
    });
    
    // Error handling tests
    suite.add_test("Handle Missing Mesh File", []() 
    {
        try 
        {
            MeshSkinner skinner;

            const bool success = skinner.load_mesh("asset/nonexistent_mesh.obj");
            
            if (success) 
            {
                std::cout << "Failed to reject nonexistent mesh file\n";
                return false;
            } 
            else 
            {
                std::cout << "Correctly rejected nonexistent mesh file\n";
                return true;
            }
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Correctly rejected nonexistent mesh file with exception: " 
                      << e.what() << std::endl;
            return true;
        }
    });
    
    suite.add_test("Handle Missing Weights File", []() 
    {
        try 
        {
            MeshSkinner skinner;

            const bool success = skinner.load_weights("asset/nonexistent_weights.json");
            
            if (success) 
            {
                std::cout << "Failed to reject nonexistent weights file\n";
                return false;
            } 
            else 
            {
                std::cout << "Correctly rejected nonexistent weights file\n";
                return true;
            }
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Correctly rejected nonexistent weights file with exception: " 
                      << e.what() << std::endl;
            return true;
        }
    });
    
    // Full pipeline tests

    suite.add_test("Complete Skinning Pipeline", []() 
    {
        std::cout << "Testing complete skinning pipeline (load, process, save)...\n";
        
        MeshSkinner skinner;

        const std::string temp_output_path = "asset/temp_skinned_output.obj";

        bool success = true;
        
        try 
        {
            // Step 1: Load all required data
            std::cout << "Step 1: Loading required data...\n";
            
            const bool loaded_mesh = skinner.load_mesh("asset/input_mesh.obj");
            std::cout << "Loaded mesh: " << (loaded_mesh ? "Yes" : "No") << std::endl;
            success &= loaded_mesh;
            
            const bool loaded_weights = skinner.load_weights("asset/bone_weights.json");
            std::cout << "Loaded weights: " << (loaded_weights ? "Yes" : "No") << std::endl;
            success &= loaded_weights;
            
            const bool loaded_bind_pose = skinner.load_bind_pose("asset/inverse_bind_pose.json");
            std::cout << "Loaded bind pose: " << (loaded_bind_pose ? "Yes" : "No") << std::endl;
            success &= loaded_bind_pose;
            
            const bool loaded_new_pose = skinner.load_new_pose("asset/output_pose.json");
            std::cout << "Loaded new pose: " << (loaded_new_pose ? "Yes" : "No") << std::endl;
            success &= loaded_new_pose;
            
            if (!success) 
            {
                std::cout << "Failed to load all required data\n";
                return false;
            }
            
            // Step 2: Perform skinning
            std::cout << "Step 2: Performing skinning operation...\n";
            const bool skinning_success = skinner.perform_skinning();
            std::cout << "Skinning completed: " << (skinning_success ? "Yes" : "No") << std::endl;
            
            if (!skinning_success) 
            {
                std::cout << "Skinning operation failed\n";
                return false;
            }
            
            // Display performance metrics
            skinner.print_timing_metrics();
            
            // Step 3: Save and validate result
            std::cout << "Step 3: Saving and validating result...\n";
            const bool save_success = skinner.save_skinned_mesh(temp_output_path);
            std::cout << "Save operation: " << (save_success ? "Success" : "Failed") << std::endl;
            
            if (!save_success) 
            {
                std::cout << "Failed to save skinned mesh\n";
                return false;
            }
            
            // Verify file exists and has valid content
            const bool file_exists = std::filesystem::exists(temp_output_path);
            std::cout << "Output file exists: " << (file_exists ? "Yes" : "No") << std::endl;
            
            if (file_exists) 
            {
                // Load the saved mesh to validate
                try 
                {
                    const Mesh saved_mesh = ObjFacade::load_obj_mesh(temp_output_path);

                    std::cout << "Saved mesh contains " << saved_mesh.vertices.size() 
                              << " vertices and " << saved_mesh.faces.size() << " faces" << std::endl;
                    
                    // Clean up
                    std::filesystem::remove(temp_output_path);

                    return !saved_mesh.vertices.empty() && !saved_mesh.faces.empty();
                } 
                catch (const std::exception& e) 
                {
                    std::cout << "Failed to validate saved mesh: " << e.what() << std::endl;
                    
                    // Clean up even if validation fails
                    std::filesystem::remove(temp_output_path);

                    return false;
                }
            }
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Pipeline test failed with exception: " << e.what() << std::endl;
            
            // Clean up in case file was created before exception
            if (std::filesystem::exists(temp_output_path)) 
            {
                std::filesystem::remove(temp_output_path);
            }
            
            return false;
        }
    });
    
    return suite;
}
