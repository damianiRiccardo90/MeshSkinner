#include "MeshSkinner.h"

// Standard library imports
#include <iostream>
#include <string>
#include <vector>

// Local application imports
#include "facade/JsonFacade.h"
#include "facade/MathFacade.h"
#include "model/Mesh.h"
#include "model/Skeleton.h"
#include "model/SkinningData.h"


const float MeshSkinner::WEIGHT_THRESHOLD = 0.0001f;

bool MeshSkinner::load_mesh(const std::string& mesh_path)
{
    try 
    {
        const Json json_data = JsonFacade::load_from_file(mesh_path);
        
        // Clear existing mesh data
        original_mesh.vertices.clear();
        original_mesh.indices.clear();
        
        // Parse vertices
        const Json vertices_json = json_data["vertices"];
        for (size_t i = 0; i < vertices_json.size(); i++) 
        {
            const Json vertex = vertices_json.at(i);
            const Vertex v{
                vertex.at(0).as_float(),
                vertex.at(1).as_float(),
                vertex.at(2).as_float()
            };
            original_mesh.vertices.push_back(v);
        }
        
        // Parse indices
        const Json indices_json = json_data["indices"];
        for (size_t i = 0; i < indices_json.size(); i++) 
        {
            original_mesh.indices.push_back(indices_json.at(i).as_int());
        }
        
        // Initialize the skinned mesh as a copy of the original
        skinned_mesh = original_mesh;
        
        std::cout << "Loaded mesh with " << original_mesh.vertices.size() << " vertices" << std::endl;
        return true;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Failed to load mesh: " << e.what() << std::endl;
        return false;
    }
}

bool MeshSkinner::load_weights(const std::string& weights_path)
{
    try 
    {
        const Json json_data = JsonFacade::load_from_file(weights_path);
        
        // Clear existing weights
        skin_data.weights.clear();
        
        // Parse weights for each vertex
        const Json vertices_weights = json_data["vertex_weights"];
        for (size_t i = 0; i < vertices_weights.size(); i++) 
        {
            std::vector<JointInfluence> influences;
            const Json vertex_influences = vertices_weights.at(i);
            
            for (size_t j = 0; j < vertex_influences.size(); j++) 
            {
                const Json influence = vertex_influences.at(j);
                JointInfluence joint_influence;
                joint_influence.joint_id = influence["joint_id"].as_int();
                joint_influence.weight = influence["weight"].as_float();
                influences.push_back(joint_influence);
            }
            
            skin_data.weights.push_back(influences);
        }
        
        std::cout << "Loaded skinning weights for " << skin_data.weights.size() << " vertices" << std::endl;
        return true;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Failed to load weights: " << e.what() << std::endl;
        return false;
    }
}

bool MeshSkinner::load_bind_pose(const std::string& bind_pose_path)
{
    try 
    {
        const Json json_data = JsonFacade::load_from_file(bind_pose_path);
        
        // Clear existing skeleton
        bind_pose.joints.clear();
        
        // Parse joints
        const Json joints_json = json_data["joints"];
        for (size_t i = 0; i < joints_json.size(); i++) 
        {
            const Json joint = joints_json.at(i);
            
            Joint j;
            j.name = joint["name"].as_string();
            j.parent_id = joint["parent_id"].as_int();
            
            // Parse local transform
            const Json transform = joint["local_transform"];
            for (int row = 0; row < 4; row++) 
            {
                for (int col = 0; col < 4; col++) 
                {
                    j.local_transform.m[row][col] = transform.at(row * 4 + col).as_float();
                }
            }
            
            bind_pose.joints.push_back(j);
        }
        
        std::cout << "Loaded bind pose with " << bind_pose.joints.size() << " joints" << std::endl;
        return true;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Failed to load bind pose: " << e.what() << std::endl;
        return false;
    }
}

bool MeshSkinner::load_new_pose(const std::string& new_pose_path)
{
    try 
    {
        const Json json_data = JsonFacade::load_from_file(new_pose_path);
        
        // Clear existing skeleton
        new_pose.joints.clear();
        
        // Parse joints
        const Json joints_json = json_data["joints"];
        for (size_t i = 0; i < joints_json.size(); i++) 
        {
            const Json joint = joints_json.at(i);
            
            Joint j;
            j.name = joint["name"].as_string();
            j.parent_id = joint["parent_id"].as_int();
            
            // Parse local transform
            const Json transform = joint["local_transform"];
            for (int row = 0; row < 4; row++) 
            {
                for (int col = 0; col < 4; col++) 
                {
                    j.local_transform.m[row][col] = transform.at(row * 4 + col).as_float();
                }
            }
            
            new_pose.joints.push_back(j);
        }
        
        std::cout << "Loaded new pose with " << new_pose.joints.size() << " joints" << std::endl;
        return true;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Failed to load new pose: " << e.what() << std::endl;
        return false;
    }
}

bool MeshSkinner::perform_skinning()
{
    // Verify all required data is loaded
    if (original_mesh.vertices.empty()) 
    {
        std::cerr << "No mesh loaded" << std::endl;
        return false;
    }
    
    if (skin_data.weights.empty()) 
    {
        std::cerr << "No skinning weights loaded" << std::endl;
        return false;
    }
    
    if (bind_pose.joints.empty() || new_pose.joints.empty()) 
    {
        std::cerr << "Bind pose or new pose not loaded" << std::endl;
        return false;
    }
    
    // Calculate and apply vertex transformations
    calculate_vertex_transformations();
    apply_vertex_transformations();
    
    std::cout << "Skinning completed successfully" << std::endl;
    return true;
}

bool MeshSkinner::save_skinned_mesh(const std::string& output_path)
{
    try 
    {
        // Create JSON structure for the skinned mesh
        Json json_data = Json::make_object();
        
        // Add vertices
        Json vertices_json = Json::make_array();
        for (const auto& vertex : skinned_mesh.vertices) 
        {
            Json vertex_json = Json::make_array();
            vertex_json.push_back(vertex.x);
            vertex_json.push_back(vertex.y);
            vertex_json.push_back(vertex.z);
            vertices_json.push_back(vertex_json);
        }
        json_data.set("vertices", vertices_json);
        
        // Add indices
        Json indices_json = Json::make_array();
        for (const auto& index : skinned_mesh.indices)
        {
            indices_json.push_back(index);
        }
        json_data.set("indices", indices_json);
        
        // Write to file using the facade
        JsonFacade::save_to_file(output_path, json_data, true);
        
        std::cout << "Saved skinned mesh to: " << output_path << std::endl;
        return true;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Failed to save skinned mesh: " << e.what() << std::endl;
        return false;
    }
}

const Mesh &MeshSkinner::get_skinned_mesh() const
{
    return skinned_mesh;
}

void MeshSkinner::calculate_vertex_transformations()
{
    // Clear existing transformation data
    skin_data.skinning_matrices.clear();
    
    // Calculate global transforms for bind pose
    std::vector<hmm_mat4> bind_global_transforms;
    bind_pose.calculate_global_transforms(bind_global_transforms);
    
    // Calculate global transforms for new pose
    std::vector<hmm_mat4> new_global_transforms;
    new_pose.calculate_global_transforms(new_global_transforms);
    
    // Calculate skinning matrices (new_pose * inverse_bind_pose)
    for (size_t i = 0; i < bind_global_transforms.size(); i++) 
    {
        const hmm_mat4 inverse_bind = 
            MathFacade::mat4_inverse(bind_global_transforms[i]);
        const hmm_mat4 skinning_matrix = 
            MathFacade::mat4_multiply(new_global_transforms[i], inverse_bind);
        
        skin_data.skinning_matrices.push_back(skinning_matrix);
    }
}

void MeshSkinner::apply_vertex_transformations()
{
    // Reset skinned mesh to have the same vertices as original
    skinned_mesh.vertices = original_mesh.vertices;
    
    // Apply linear blend skinning to each vertex
    for (size_t i = 0; i < original_mesh.vertices.size(); i++) 
    {
        const hmm_vec3 original_position = {
            original_mesh.vertices[i].x,
            original_mesh.vertices[i].y,
            original_mesh.vertices[i].z
        };
        
        hmm_vec3 new_position = {0.0f, 0.0f, 0.0f};
        
        // Get the bone influences for this vertex
        const auto& vertex_weights = skin_data.weights[i];
        
        // Blend transformations based on weights
        for (const auto& influence : vertex_weights) 
        {
            const int joint_id = influence.joint_id;
            const float weight = influence.weight;
            
            // Skip influences with negligible weights
            if (weight < WEIGHT_THRESHOLD) continue;
            
            // Transform position by the skinning matrix
            const hmm_vec3 transformed_position = MathFacade::transform_point(
                skin_data.skinning_matrices[joint_id], original_position);
            
            // Add weighted contribution to the final position
            new_position.X += transformed_position.X * weight;
            new_position.Y += transformed_position.Y * weight;
            new_position.Z += transformed_position.Z * weight;
        }
        
        // Update the skinned mesh vertex
        skinned_mesh.vertices[i].x = new_position.X;
        skinned_mesh.vertices[i].y = new_position.Y;
        skinned_mesh.vertices[i].z = new_position.Z;
    }
}
