// Standard library imports
#include <filesystem>
#include <iostream>

// Local application imports
#include "facade/obj_facade.h"
#include "mesh_skinner.h"
#include "model/mesh.h"
#include "test/test_framework.h"
#include "test/test_utils.h"


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
            
            if (success) 
            {
                TestUtils::print_colored("Loading mesh: Success\n", 
                    TestUtils::ConsoleColor::Green);
            } 
            else 
            {
                TestUtils::print_colored("Loading mesh: Failed\n", 
                    TestUtils::ConsoleColor::Red);
            }
            return success;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Loading mesh failed with exception: " << e.what() << std::endl;
            TestUtils::reset_console_color();
            return false;
        }
    });
    
    suite.add_test("Load Valid Weights", []() 
    {
        try 
        {
            MeshSkinner skinner;

            const bool success = skinner.load_weights("asset/bone_weights.json");
            
            if (success) 
            {
                TestUtils::print_colored("Loading weights: Success\n", 
                    TestUtils::ConsoleColor::Green);
            } 
            else 
            {
                TestUtils::print_colored("Loading weights: Failed\n", 
                    TestUtils::ConsoleColor::Red);
            }
            return success;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Loading weights failed with exception: " << e.what() << std::endl;
            TestUtils::reset_console_color();
            return false;
        }
    });
    
    suite.add_test("Load Valid Bind Pose", []() 
    {
        try 
        {
            MeshSkinner skinner;

            const bool success = skinner.load_bind_pose("asset/inverse_bind_pose.json");
            
            if (success) 
            {
                TestUtils::print_colored("Loading bind pose: Success\n", 
                    TestUtils::ConsoleColor::Green);
            } 
            else 
            {
                TestUtils::print_colored("Loading bind pose: Failed\n", 
                    TestUtils::ConsoleColor::Red);
            }
            return success;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Loading bind pose failed with exception: " << e.what() << std::endl;
            TestUtils::reset_console_color();
            return false;
        }
    });
    
    suite.add_test("Load Valid New Pose", []() 
    {
        try 
        {
            MeshSkinner skinner;

            const bool success = skinner.load_new_pose("asset/output_pose.json");
            
            if (success) 
            {
                TestUtils::print_colored("Loading new pose: Success\n", 
                    TestUtils::ConsoleColor::Green);
            } 
            else 
            {
                TestUtils::print_colored("Loading new pose: Failed\n", 
                    TestUtils::ConsoleColor::Red);
            }
            return success;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Loading new pose failed with exception: " << e.what() << std::endl;
            TestUtils::reset_console_color();
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
                TestUtils::print_colored("Failed to reject nonexistent mesh file\n", 
                    TestUtils::ConsoleColor::Red);
                return false;
            } 
            else 
            {
                TestUtils::print_colored("Correctly rejected nonexistent mesh file\n", 
                    TestUtils::ConsoleColor::Green);
                return true;
            }
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Green);
            std::cout << "Correctly rejected nonexistent mesh file with exception: " 
                      << e.what() << std::endl;
            TestUtils::reset_console_color();
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
                TestUtils::print_colored("Failed to reject nonexistent weights file\n", 
                    TestUtils::ConsoleColor::Red);
                return false;
            } 
            else 
            {
                TestUtils::print_colored("Correctly rejected nonexistent weights file\n", 
                    TestUtils::ConsoleColor::Green);
                return true;
            }
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Green);
            std::cout << "Correctly rejected nonexistent weights file with exception: " 
                      << e.what() << std::endl;
            TestUtils::reset_console_color();
            return true;
        }
    });
    
    // Full pipeline tests
    suite.add_test("Complete Skinning Pipeline", []() 
    {
        TestUtils::print_colored("Testing complete skinning pipeline (load, process, save)...\n", 
            TestUtils::ConsoleColor::Default);
        
        MeshSkinner skinner;

        const std::string temp_output_path = "asset/temp_skinned_output.obj";

        bool success = true;
        
        try 
        {
            // Step 1: Load all required data
            TestUtils::print_colored("Step 1: Loading required data...\n", 
                TestUtils::ConsoleColor::Default);
            
            const bool loaded_mesh = skinner.load_mesh("asset/input_mesh.obj");
            TestUtils::set_console_color(
                loaded_mesh ? TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
            std::cout << "Loaded mesh: " << (loaded_mesh ? "Yes" : "No") << std::endl;
            TestUtils::reset_console_color();
            success &= loaded_mesh;
            
            const bool loaded_weights = skinner.load_weights("asset/bone_weights.json");
            TestUtils::set_console_color(
                loaded_weights ? TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
            std::cout << "Loaded weights: " << (loaded_weights ? "Yes" : "No") << std::endl;
            TestUtils::reset_console_color();
            success &= loaded_weights;
            
            const bool loaded_bind_pose = skinner.load_bind_pose("asset/inverse_bind_pose.json");
            TestUtils::set_console_color(
                loaded_bind_pose ? TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
            std::cout << "Loaded bind pose: " << (loaded_bind_pose ? "Yes" : "No") << std::endl;
            TestUtils::reset_console_color();
            success &= loaded_bind_pose;
            
            const bool loaded_new_pose = skinner.load_new_pose("asset/output_pose.json");
            TestUtils::set_console_color(
                loaded_new_pose ? TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
            std::cout << "Loaded new pose: " << (loaded_new_pose ? "Yes" : "No") << std::endl;
            TestUtils::reset_console_color();
            success &= loaded_new_pose;
            
            if (!success) 
            {
                TestUtils::print_colored("Failed to load all required data\n", 
                    TestUtils::ConsoleColor::Red);
                return false;
            }
            else
            {
                TestUtils::print_colored("Successfully loaded all required data\n", 
                    TestUtils::ConsoleColor::Green);
            }
            
            // Step 2: Perform skinning
            TestUtils::print_colored("Step 2: Performing skinning operation...\n", 
                TestUtils::ConsoleColor::Default);
            const bool skinning_success = skinner.perform_skinning();
            
            if (skinning_success) 
            {
                TestUtils::print_colored("Skinning completed successfully\n", 
                    TestUtils::ConsoleColor::Green);
            } 
            else 
            {
                TestUtils::print_colored("Skinning operation failed\n", 
                    TestUtils::ConsoleColor::Red);
                return false;
            }
            
            // Display performance metrics
            std::cout << "\nPerformance metrics:\n";
            skinner.print_timing_metrics();
            
            // Step 3: Save and validate result
            TestUtils::print_colored("Step 3: Saving and validating result...\n", 
                TestUtils::ConsoleColor::Default);
            const bool save_success = skinner.save_skinned_mesh(temp_output_path);
            
            if (save_success) 
            {
                TestUtils::print_colored("Save operation: Success\n", 
                    TestUtils::ConsoleColor::Green);
            } 
            else 
            {
                TestUtils::print_colored("Save operation: Failed\n", 
                    TestUtils::ConsoleColor::Red);
                return false;
            }
            
            // Verify file exists and has valid content
            const bool file_exists = std::filesystem::exists(temp_output_path);
            
            TestUtils::set_console_color(
                file_exists ? TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
            std::cout << "Output file exists: " << (file_exists ? "Yes" : "No") << std::endl;
            TestUtils::reset_console_color();
            
            if (file_exists) 
            {
                // Load the saved mesh to validate
                try 
                {
                    const Mesh saved_mesh = ObjFacade::load_obj_mesh(temp_output_path);

                    TestUtils::set_console_color(TestUtils::ConsoleColor::Green);
                    std::cout << "Saved mesh contains " << saved_mesh.vertices.size() 
                              << " vertices and " << saved_mesh.faces.size() << " faces" << std::endl;
                    TestUtils::reset_console_color();
                    
                    // Clean up
                    std::filesystem::remove(temp_output_path);

                    const bool valid_mesh = !saved_mesh.vertices.empty() && !saved_mesh.faces.empty();
                    
                    if (valid_mesh) 
                    {
                        TestUtils::print_colored("Complete skinning pipeline test passed!\n", 
                            TestUtils::ConsoleColor::Green);
                    } 
                    else 
                    {
                        TestUtils::print_colored("Saved mesh validation failed - empty mesh\n", 
                            TestUtils::ConsoleColor::Red);
                    }

                    return valid_mesh;
                } 
                catch (const std::exception& e) 
                {
                    TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
                    std::cout << "Failed to validate saved mesh: " << e.what() << std::endl;
                    TestUtils::reset_console_color();
                    
                    // Clean up even if validation fails
                    std::filesystem::remove(temp_output_path);

                    return false;
                }
            }
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Pipeline test failed with exception: " << e.what() << std::endl;
            TestUtils::reset_console_color();
            
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
