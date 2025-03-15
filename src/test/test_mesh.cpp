// Standard library imports
#include <filesystem>
#include <fstream>
#include <iostream>

// Third-party imports
#include "handmade_math/handmade_math.h"

// Local application imports
#include "facade/obj_facade.h"
#include "model/mesh.h"
#include "test/test_framework.h"
#include "test/test_utils.h"


TestSuite create_mesh_tests() 
{
    TestSuite suite("Mesh Tests");

    // Basic file loading test
    suite.add_test("Load Valid OBJ File", []() 
    {
        try 
        {
            const Mesh mesh = ObjFacade::load_obj_mesh("asset/cube.obj");

            TestUtils::print_colored("Loading cube.obj: Success\n", 
                TestUtils::ConsoleColor::Green);
            return !mesh.vertices.empty();
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Loading cube.obj failed: " << e.what() << std::endl;
            TestUtils::reset_console_color();
            return false;
        }
    });
    
    // Edge case: Nonexistent file
    suite.add_test("Handle Nonexistent File", []() 
    {
        try 
        {
            ObjFacade::load_obj_mesh("asset/nonexistent.obj");

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
    
    // Edge case: Empty file
    suite.add_test("Handle Empty File", []() 
    {
        // Create a temporary empty file
        const std::string tempFilePath = "asset/temp_empty.obj";
        std::ofstream tempFile(tempFilePath);
        
        try 
        {
            ObjFacade::load_obj_mesh(tempFilePath);
            TestUtils::print_colored("Failed to reject empty file\n", 
                TestUtils::ConsoleColor::Red);
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::print_colored("Correctly rejected empty file\n", 
                TestUtils::ConsoleColor::Green);
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return true;
        }
    });
    
    // Edge case: File with only headers/comments but no geometry
    suite.add_test("Handle File With Only Headers", []() 
    {
        // Create a temporary file with only headers
        const std::string tempFilePath = "asset/temp_headers_only.obj";
        std::ofstream tempFile(tempFilePath);
        tempFile << "# This is a comment\n";
        tempFile << "# Another comment line\n";
        tempFile << "mtllib materials.mtl\n";
        tempFile << "o TestObject\n";
        // No actual geometry data
        
        try 
        {
            ObjFacade::load_obj_mesh(tempFilePath);
            TestUtils::print_colored("Failed to handle file with only headers\n", 
                TestUtils::ConsoleColor::Red);
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::print_colored("Correctly handled file with only headers\n", 
                TestUtils::ConsoleColor::Green);
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return true;
        }
    });
    
    // Comprehensive mesh validation
    suite.add_test("Validate Mesh Properties", []() 
    {
        Mesh mesh;

        try 
        {
            mesh = ObjFacade::load_obj_mesh("test_data/cube.obj");
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Failed to load test cube: " << e.what() << std::endl;
            TestUtils::reset_console_color();
            return false;
        }
        
        bool all_tests_passed = true;
        
        // Check vertices
        std::cout << "Vertex count: " << mesh.vertices.size() << " (expected 8 for a cube)\n";
        if (mesh.vertices.size() != 8) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "ERROR: Unexpected vertex count. Expected 8, got " 
                      << mesh.vertices.size() << std::endl;
            TestUtils::reset_console_color();
            all_tests_passed = false;
        }
        else
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Green);
            std::cout << "Vertex count matches expected value (8)\n";
            TestUtils::reset_console_color();
        }
        
        // Check faces (count and structure)
        std::cout << "Face count: " << mesh.faces.size() << std::endl;
        if (mesh.faces.empty()) 
        {
            TestUtils::print_colored("ERROR: No faces found in the mesh\n", 
                TestUtils::ConsoleColor::Red);
            all_tests_passed = false;
        } 
        else 
        {
            // A cube should have 12 triangular faces (6 sides, 2 triangles per side)
            if (mesh.faces.size() != 12) 
            {
                TestUtils::set_console_color(TestUtils::ConsoleColor::Yellow);
                std::cout << "WARNING: Unexpected number of faces for a cube. Expected 12, got " 
                          << mesh.faces.size() << std::endl;
                TestUtils::reset_console_color();
            }
            else
            {
                TestUtils::print_colored("Face count matches expected value (12)\n", 
                    TestUtils::ConsoleColor::Green);
            }
            
            // Verify all faces have valid indices (within vertex array bounds)
            bool has_invalid_indices = false;
            for (size_t i = 0; i < mesh.faces.size(); i++) 
            {
                const auto& face = mesh.faces[i];
                for (size_t j = 0; j < 3; j++) // Face struct uses array of 3 indices
                {
                    if (face.indices[j] >= mesh.vertices.size()) 
                    {
                        TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
                        std::cout << "ERROR: Face " << i << " has invalid vertex index: " 
                                  << face.indices[j] << " (max valid: " 
                                  << mesh.vertices.size() - 1 << ")\n";
                        TestUtils::reset_console_color();
                        has_invalid_indices = true;
                        all_tests_passed = false;
                    }
                }
            }
            
            if (!has_invalid_indices) 
            {
                TestUtils::print_colored("All faces have valid vertex indices\n", 
                    TestUtils::ConsoleColor::Green);
            }
        }

        // Check for disconnected vertices
        bool has_disconnected = false;
        for (size_t i = 0; i < mesh.vertices.size(); i++) 
        {
            if (!TestUtils::is_vertex_connected(mesh.faces, i)) 
            {
                if (!has_disconnected) 
                {
                    // Print header only on first disconnected vertex
                    TestUtils::print_colored("Disconnected vertices found:\n", 
                        TestUtils::ConsoleColor::Red);
                }
                has_disconnected = true;
                
                TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
                std::cout << "  - Vertex at index " << i << " (position: " 
                          << mesh.vertices[i].x << ", " 
                          << mesh.vertices[i].y << ", " 
                          << mesh.vertices[i].z << ")\n";
                TestUtils::reset_console_color();
                
                all_tests_passed = false;
            }
        }
        
        if (!has_disconnected) 
        {
            TestUtils::print_colored("All vertices are connected to at least one face\n", 
                TestUtils::ConsoleColor::Green);
        } 
        else 
        {
            TestUtils::print_colored(
                "WARNING: Mesh has disconnected vertices which may indicate loading issues\n", 
                TestUtils::ConsoleColor::Yellow
            );
        }

        // Check mesh bounds
        std::vector<HMM_Vec3> vertexPositions;
        vertexPositions.reserve(mesh.vertices.size());
        for (const auto& vertex : mesh.vertices) 
        {
            vertexPositions.push_back(HMM_V3(vertex.x, vertex.y, vertex.z));
        }

        const auto [min_bounds, max_bounds] = TestUtils::calculate_mesh_bounds(vertexPositions);
        std::cout << "Mesh bounds: Min(" 
                  << min_bounds.X << ", " << min_bounds.Y << ", " << min_bounds.Z << ") Max("
                  << max_bounds.X << ", " << max_bounds.Y << ", " << max_bounds.Z << ")\n";
        
        if (all_tests_passed)
        {
            TestUtils::print_colored("All mesh validation tests passed!\n", 
                TestUtils::ConsoleColor::Green);
        }
        else
        {
            TestUtils::print_colored("Some mesh validation tests failed.\n", 
                TestUtils::ConsoleColor::Red);
        }
        
        return all_tests_passed;
    });
    
    // Add test for saving mesh
    suite.add_test("Save and Reload Mesh", []() 
    {
        try 
        {
            // Load original mesh
            Mesh original_mesh = ObjFacade::load_obj_mesh("test_data/cube.obj");
            
            // Save to a temporary file
            const std::string temp_save_path = "test_data/temp_save_test.obj";
            
            if (!ObjFacade::save_obj_mesh(temp_save_path, original_mesh)) 
            {
                TestUtils::print_colored("Failed to save mesh\n", TestUtils::ConsoleColor::Red);
                return false;
            }
            
            // Reload the saved mesh
            const Mesh reloaded_mesh = ObjFacade::load_obj_mesh(temp_save_path);
            
            // Verify vertex counts match
            const bool vertices_match = reloaded_mesh.vertices.size() == original_mesh.vertices.size();
            
            TestUtils::set_console_color(vertices_match ? 
                TestUtils::ConsoleColor::Green : TestUtils::ConsoleColor::Red);
            std::cout << "Vertex count matches after save/reload: " 
                      << (vertices_match ? "Yes" : "No") << std::endl;
            TestUtils::reset_console_color();
            
            // Verify face counts match
            const bool faces_match = reloaded_mesh.faces.size() == original_mesh.faces.size();
            
            TestUtils::set_console_color(faces_match ? TestUtils::ConsoleColor::Green : 
                TestUtils::ConsoleColor::Red);
            std::cout << "Face count matches after save/reload: " 
                      << (faces_match ? "Yes" : "No") << std::endl;
            TestUtils::reset_console_color();
            
            // Clean up
            std::filesystem::remove(temp_save_path);
            
            if (vertices_match && faces_match)
            {
                TestUtils::print_colored("Save and reload test passed successfully!\n", 
                    TestUtils::ConsoleColor::Green);
            }
            
            return vertices_match && faces_match;
        } 
        catch (const std::exception& e) 
        {
            TestUtils::set_console_color(TestUtils::ConsoleColor::Red);
            std::cout << "Save and reload test failed: " << e.what() << std::endl;
            TestUtils::reset_console_color();
            return false;
        }
    });
    
    return suite;
}
