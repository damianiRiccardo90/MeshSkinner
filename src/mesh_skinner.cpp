#include "mesh_skinner.h"

// Standard library imports
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

// Third-party imports
#include "indicators/indicators.h"

// Local application imports
#include "facade/json_facade.h"
#include "facade/math_facade.h"
#include "facade/obj_facade.h"
#include "model/mesh.h"
#include "model/skeleton.h"
#include "model/skinning_data.h"


// Threshold below which joint weights are considered negligible.
const float MeshSkinner::WEIGHT_THRESHOLD = .0001f;

bool MeshSkinner::load_mesh(const std::string& mesh_path)
{
    try
    {
        // Load JSON from file
        original_mesh = ObjFacade::load_obj_mesh(mesh_path);

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
        // Load JSON from file
        const Json json_data = JsonFacade::load_from_file(weights_path);

        // Parse out the weights
        skin_data.weights = SkinningData::from_json(json_data);

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

bool MeshSkinner::load_bind_pose(const std::string& bind_pose_path)
{
    try
    {
        // Load JSON from file
        const Json json_data = JsonFacade::load_from_file(bind_pose_path);

        // Parse it into the bind pose Skeleton
        bind_pose = Skeleton::from_json(json_data);

        std::cout << "Loaded bind pose with " << bind_pose.joints.size() << " joints.\n";
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
        // Load JSON from file
        const Json json_data = JsonFacade::load_from_file(new_pose_path);
        
        // Parse it into the new pose Skeleton
        new_pose = Skeleton::from_json(json_data);

        std::cout << "Loaded new pose with " << new_pose.joints.size() << " joints.\n";
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

    // Verify weights count matches vertex count
    if (skin_data.weights.size() != original_mesh.vertices.size()) 
    {
        std::cerr << "Weight data count (" << skin_data.weights.size() 
                  << ") doesn't match vertex count (" 
                  << original_mesh.vertices.size() << ")" << std::endl;
        return false;
    }

    // Record the total skinning time
    const auto total_start = std::chrono::high_resolution_clock::now();

    // Calculate and apply vertex transformations with timing

    const auto calc_start = std::chrono::high_resolution_clock::now();
    calculate_vertex_transformations(); // Function to be measured
    const auto calc_end = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli> calc_duration = calc_end - calc_start;
    record_timing("Calculate Transformations", calc_duration.count());
    
    const auto apply_start = std::chrono::high_resolution_clock::now();
    apply_vertex_transformations(); // Function to be measured
    const auto apply_end = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli> apply_duration = apply_end - apply_start;
    record_timing("Apply Transformations", apply_duration.count());

    const auto total_end = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli> total_duration = total_end - total_start;
    record_timing("Total Skinning Time", total_duration.count());
    
    std::cout << "Skinning completed successfully" << std::endl;
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

void MeshSkinner::calculate_vertex_transformations()
{
    // Clear existing transformation data
    skin_data.skinning_matrices.clear();
    
    // Calculate global transforms for bind pose (while recording timing)
    const auto bind_start = std::chrono::high_resolution_clock::now();
    std::vector<HMM_Mat4> bind_global_transforms;
    bind_pose.calculate_global_transforms(bind_global_transforms);
    const auto bind_end = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli> bind_duration = bind_end - bind_start;
    record_timing("Calculate Bind Global Transforms", bind_duration.count());
    
    // Calculate global transforms for new pose (while recording timing)
    const auto new_start = std::chrono::high_resolution_clock::now();
    std::vector<HMM_Mat4> new_global_transforms;
    new_pose.calculate_global_transforms(new_global_transforms);
    const auto new_end = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli> new_duration = new_end - new_start;
    record_timing("Calculate New Global Transforms", new_duration.count());
    
    // Calculate skinning matrices (new_pose * inverse_bind_pose)
    const auto matrices_start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < bind_global_transforms.size(); i++) 
    {
        const HMM_Mat4 inverse_bind = 
            MathFacade::inverse(bind_global_transforms[i]);
        const HMM_Mat4 skinning_matrix = 
            MathFacade::multiply(new_global_transforms[i], inverse_bind);
        
        skin_data.skinning_matrices.push_back(skinning_matrix);
    }
    const auto matrices_end = std::chrono::high_resolution_clock::now();

    const std::chrono::duration<double, std::milli> matrices_duration = matrices_end - matrices_start;
    record_timing("Calculate Skinning Matrices", matrices_duration.count());
}

void MeshSkinner::apply_vertex_transformations()
{
    // Reset skinned mesh to have the same vertices as original
    skinned_mesh.vertices = original_mesh.vertices;

    const size_t total_vertices = original_mesh.vertices.size();

    // Setup the progress bar
    indicators::ProgressBar progress_bar{
        indicators::option::BarWidth{50},
        indicators::option::Start{"["},
        indicators::option::End{"]"},
        indicators::option::ForegroundColor{indicators::Color::green},
        indicators::option::ShowElapsedTime{true},
        indicators::option::ShowPercentage{true},
        indicators::option::PrefixText{"Applying skinning"}
    };

    // Track the skinning loop timing
    const auto skinning_start = std::chrono::high_resolution_clock::now();
    
    // Apply linear blend skinning to each vertex
    for (size_t i = 0; i < total_vertices; i++) 
    {
        const HMM_Vec3 original_position = HMM_V3(
            original_mesh.vertices[i].x,
            original_mesh.vertices[i].y,
            original_mesh.vertices[i].z
        );
        
        HMM_Vec3 new_position = HMM_V3(0.f, 0.f, 0.f);
        
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
            const HMM_Vec3 transformed_position = MathFacade::transform_vec3(
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

        // Update progress bar
        progress_bar.set_progress(static_cast<float>(i + 1) / total_vertices * 100);
    }

    const auto skinning_end = std::chrono::high_resolution_clock::now();
    const std::chrono::duration<double, std::milli> skinning_duration = skinning_end - skinning_start;
    record_timing("Vertex Skinning Loop", skinning_duration.count());

    // Ensure progress bar shows 100% at the end
    progress_bar.set_progress(100);
    std::cout << std::endl << "Skinning completed successfully" << std::endl;
}

void MeshSkinner::record_timing(const std::string& operation_name, double duration)
{
    timing_metrics[operation_name] = duration;
}
