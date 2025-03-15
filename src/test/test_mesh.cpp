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

            std::cout << "Loading cube.obj: Success\n";
            return !mesh.vertices.empty();
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Loading cube.obj failed: " << e.what() << std::endl;
            return false;
        }
    });
    
    // Edge case: Nonexistent file
    suite.add_test("Handle Nonexistent File", []() 
    {
        try 
        {
            ObjFacade::load_obj_mesh("asset/nonexistent.obj");

            std::cout << "Failed to reject nonexistent file\n";
            return false;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Correctly rejected nonexistent file\n";
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
            std::cout << "Failed to reject empty file\n";
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Correctly rejected empty file\n";
            
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
            std::cout << "Failed to handle file with only headers\n";
            
            // Clean up
            std::filesystem::remove(tempFilePath);
            
            return false;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Correctly handled file with only headers\n";
            
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
            std::cout << "Failed to load test cube: " << e.what() << std::endl;
            return false;
        }
        
        bool all_tests_passed = true;
        
        // Check vertices
        std::cout << "Vertex count: " << mesh.vertices.size() << " (expected 8 for a cube)\n";
        if (mesh.vertices.size() != 8) 
        {
            all_tests_passed = false;
        }
        
        // Check faces (count and structure)
        std::cout << "Face count: " << mesh.faces.size() << std::endl;
        if (mesh.faces.empty()) 
        {
            std::cout << "ERROR: No faces found in the mesh" << std::endl;
            all_tests_passed = false;
        } 
        else 
        {
            // A cube should have 12 triangular faces (6 sides, 2 triangles per side)
            if (mesh.faces.size() != 12) 
            {
                std::cout << "WARNING: Unexpected number of faces for a cube. Expected 12, got " 
                          << mesh.faces.size() << std::endl;
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
                        std::cout << "ERROR: Face " << i << " has invalid vertex index: " 
                                  << face.indices[j] << " (max valid: " 
                                  << mesh.vertices.size() - 1 << ")\n";
                        has_invalid_indices = true;
                        all_tests_passed = false;
                    }
                }
            }
            
            if (!has_invalid_indices) 
            {
                std::cout << "All faces have valid vertex indices\n";
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
                    std::cout << "Disconnected vertices found:\n";
                }
                has_disconnected = true;
                std::cout << "  - Vertex at index " << i << " (position: " 
                          << mesh.vertices[i].x << ", " 
                          << mesh.vertices[i].y << ", " 
                          << mesh.vertices[i].z << ")\n";
                all_tests_passed = false;
            }
        }
        
        if (!has_disconnected) 
        {
            std::cout << "All vertices are connected to at least one face\n";
        } 
        else 
        {
            std::cout << "WARNING: Mesh has disconnected vertices which may indicate loading issues" 
                      << std::endl;
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
                std::cout << "Failed to save mesh\n";
                return false;
            }
            
            // Reload the saved mesh
            const Mesh reloaded_mesh = ObjFacade::load_obj_mesh(temp_save_path);
            
            // Verify vertex counts match
            const bool vertices_match = reloaded_mesh.vertices.size() == original_mesh.vertices.size();
            std::cout << "Vertex count matches after save/reload: " 
                      << (vertices_match ? "Yes" : "No") << std::endl;
            
            // Verify face counts match
            const bool faces_match = reloaded_mesh.faces.size() == original_mesh.faces.size();
            std::cout << "Face count matches after save/reload: " 
                      << (faces_match ? "Yes" : "No") << std::endl;
            
            // Clean up
            std::filesystem::remove(temp_save_path);
            
            return vertices_match && faces_match;
        } 
        catch (const std::exception& e) 
        {
            std::cout << "Save and reload test failed: " << e.what() << std::endl;
            return false;
        }
    });
    
    return suite;
}
