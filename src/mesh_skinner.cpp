#include "mesh_skinner.h"

// Standard library imports
#include <algorithm>
#include <chrono>
#include <execution>
#include <iostream>
#include <vector>

// Local application imports
#include "facade/json_facade.h"
#include "facade/math_facade.h"
#include "facade/obj_facade.h"


// Threshold below which joint weights are considered negligible.
const float MeshSkinner::WEIGHT_THRESHOLD = .0001f;

bool MeshSkinner::load_mesh(const std::string& mesh_path)
{
    try
    {
        // Load mesh from OBJ file
        original_mesh = ObjFacade::load_obj_mesh(mesh_path);
        // Start with a clean canvas
        skinned_mesh = original_mesh;

        std::cout << "Loaded mesh with " << original_mesh.vertices.size() 
                  << " vertices from OBJ.\n";
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
        // Load JSON weights data
        const Json json_data = JsonFacade::load_from_file(weights_path);
        
        // Parse weights data
        skin_data.weights = SkinningData::parse_weights_from_json(json_data);

        std::cout << "Loaded skinning weights for " << skin_data.weights.size()
                  << " vertices.\n";
        return true;
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Failed to load weights: " << e.what() << std::endl;
        return false;
    }
}

bool MeshSkinner::load_inverse_bind_matrices(const std::string& inv_bind_path)
{
    try
    {
        // Load JSON from file
        const Json json_data = JsonFacade::load_from_file(inv_bind_path);
        
        // Parse matrices
        skin_data.inverse_bind_matrices = SkinningData::parse_matrices_from_json(json_data);

        std::cout << "Loaded " << skin_data.inverse_bind_matrices.size() 
                  << " inverse bind matrices.\n";
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to load inverse bind matrices: " << e.what() << std::endl;
        return false;
    }
}

bool MeshSkinner::load_output_pose_matrices(const std::string& pose_path)
{
    try
    {
        // Load JSON from file
        const Json json_data = JsonFacade::load_from_file(pose_path);
        
        // Parse matrices
        skin_data.pose_matrices = SkinningData::parse_matrices_from_json(json_data);

        std::cout << "Loaded " << skin_data.pose_matrices.size() 
                  << " pose matrices.\n";
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to load pose matrices: " << e.what() << std::endl;
        return false;
    }
}

bool MeshSkinner::perform_skinning()
{
    // Verify all required data is loaded
    if (original_mesh.vertices.empty()) 
    {
        std::cerr << "No mesh loaded\n";
        return false;
    }
    
    if (skin_data.weights.empty()) 
    {
        std::cerr << "No skinning weights loaded\n";
        return false;
    }
    
    if (skin_data.inverse_bind_matrices.empty() || skin_data.pose_matrices.empty()) 
    {
        std::cerr << "Inverse bind matrices or pose matrices not loaded\n";
        return false;
    }

    // Verify weights count matches vertex count
    if (skin_data.weights.size() != original_mesh.vertices.size()) 
    {
        std::cerr << "Weight data count (" << skin_data.weights.size() 
                  << ") doesn't match vertex count (" 
                  << original_mesh.vertices.size() << ")\n";
        return false;
    }

    // Precompute skinning matrices for each joint
    const size_t joint_count = skin_data.pose_matrices.size();
    std::vector<HMM_Mat4> precomputed_matrices(joint_count);

    for (size_t joint_id = 0; joint_id < joint_count; joint_id++)
    {
        // Combine pose and inverse bind into a single matrix
        precomputed_matrices[joint_id] = MathFacade::multiply(
            skin_data.pose_matrices[joint_id],
            skin_data.inverse_bind_matrices[joint_id]
        );
    }

    std::cout << "Applying vertex transformations...\n";

    // Apply transformations using the precomputed matrices (with timing)
    const auto apply_start = std::chrono::high_resolution_clock::now();
    apply_vertex_transformations(precomputed_matrices);
    const auto apply_end = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli> apply_duration = apply_end - apply_start;
    record_timing("Apply Transformations", apply_duration.count());

    std::cout << "Skinning completed successfully\n";
    return true;
}

bool MeshSkinner::save_skinned_mesh(const std::string& output_path)
{
    try
    {
        if (!ObjFacade::save_obj_mesh(output_path, skinned_mesh))
        {
            std::cerr << "Failed to save skinned mesh to OBJ.\n";
            return false;
        }
        std::cout << "Saved skinned mesh to: " << output_path << std::endl;
        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Failed to save skinned mesh: " << e.what() << std::endl;
        return false;
    }
}

void MeshSkinner::print_timing_metrics() const
{
    if (timing_metrics.empty())
    {
        std::cout << "No timing metrics available." << std::endl;
        return;
    }
    
    std::cout << "\n===== Performance Timing Metrics =====\n";
    std::cout << std::left << std::setw(35) << "Operation" 
              << std::right << std::setw(15) << "Time (ms)" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    for (const auto& metric : timing_metrics)
    {
        std::cout << std::left << std::setw(35) << metric.first 
                  << std::right << std::setw(15) << std::fixed << std::setprecision(3) 
                  << metric.second << std::endl;
    }
    std::cout << std::string(50, '-') << std::endl;
}

void MeshSkinner::apply_vertex_transformations(const std::vector<HMM_Mat4>& precomputed_matrices)
{
    // Reset skinned mesh to have the same vertices as original
    skinned_mesh.vertices = original_mesh.vertices;

    // Parallel transform of each vertex
    std::for_each(
        std::execution::par, 
        skinned_mesh.vertices.begin(),
        skinned_mesh.vertices.end(),
        [&](Vertex& vert)
        {
            const size_t i = &vert - &skinned_mesh.vertices[0];

            const HMM_Vec3 original_position = HMM_V3(
                original_mesh.vertices[i].x,
                original_mesh.vertices[i].y,
                original_mesh.vertices[i].z
            );

            HMM_Vec3 new_position = HMM_V3(0.f, 0.f, 0.f);

            // Grab the bone influences for this vertex
            const VertexWeights& weights_for_vertex = skin_data.weights[i];

            // Accumulate contributions from each joint influence
            for (size_t j = 0; j < VertexWeights::MAX_INFLUENCES; j++)
            {
                const int joint_id = weights_for_vertex.joint_ids[j];
                const float weight = weights_for_vertex.weights[j];

                // Skip negligible weights or invalid IDs
                if (weight < WEIGHT_THRESHOLD || joint_id < 0) 
                    continue;

                const HMM_Mat4& skinning_matrix = precomputed_matrices[joint_id];
                const HMM_Vec3 transformed_position =
                    MathFacade::transform_vec3(skinning_matrix, original_position);

                // Weighted sum
                new_position.X += transformed_position.X * weight;
                new_position.Y += transformed_position.Y * weight;
                new_position.Z += transformed_position.Z * weight;
            }

            // Update the skinned vertex position
            vert.x = new_position.X;
            vert.y = new_position.Y;
            vert.z = new_position.Z;
        }
    );
}

void MeshSkinner::record_timing(const std::string& operation_name, double duration)
{
    timing_metrics[operation_name] = duration;
}
