#pragma once

// Standard library imports
#include <chrono>
#include <string>
#include <unordered_map>

// Local application imports
#include "model/mesh.h"
#include "model/skeleton.h"
#include "model/skinning_data.h"


/**
 * @brief A class for performing linear blend skinning on 3D meshes.
 *
 * This class manages the skinning process from loading input data (mesh, skeleton, weights)
 * to applying the transformations and outputting the deformed mesh. It implements
 * a complete workflow for mesh deformation using skeleton-based animation.
 */
class MeshSkinner
{
public:
        
    /**
     * @brief Loads mesh data from an OBJ file via ObjFacade.
     * @param mesh_path The path to the OBJ file.
     * @return true if the mesh was loaded successfully; otherwise false.
     */
    bool load_mesh(const std::string& mesh_path);

    /**
     * @brief Loads skinning weights from a JSON file.
     * @param weights_path The path to the weights JSON file.
     * @return true if the weights were loaded successfully; otherwise false.
     */
    bool load_weights(const std::string& weights_path);

    /**
     * @brief Loads bind pose skeleton from a JSON file.
     * @param bind_pose_path The path to the bind pose JSON file.
     * @return true if the bind pose was loaded successfully; otherwise false.
     */
    bool load_bind_pose(const std::string& bind_pose_path);

    /**
     * @brief Loads new pose skeleton from a JSON file.
     * @param new_pose_path The path to the new pose JSON file.
     * @return true if the new pose was loaded successfully; otherwise false.
     */
    bool load_new_pose(const std::string& new_pose_path);
    
    /**
     * @brief Performs the skinning operation using loaded data.
     * @return true if skinning was successful; otherwise false.
     */
    bool perform_skinning();
    
    /**
     * @brief Saves the skinned mesh to an OBJ file via ObjFacade.
     * @param output_path The path where the OBJ file will be saved.
     * @return true if the mesh was saved successfully; otherwise false.
     */
    bool save_skinned_mesh(const std::string& output_path);

    /**
     * @brief Prints all collected performance timing metrics to the console.
     */
    void print_timing_metrics() const;

protected:
    
    /**
     * @brief Calculates the skinning matrices from bind pose to new pose.
     */
    void calculate_vertex_transformations();

    /**
     * @brief Applies the skinning matrices to deform the vertices.
     */
    void apply_vertex_transformations();

    /**
     * @brief Records the execution time of an operation.
     * @param operation_name The name of the operation being timed.
     * @param duration The duration in milliseconds.
     */
    void record_timing(const std::string& operation_name, double duration);

    // Threshold below which joint weights are considered negligible.
    static const float WEIGHT_THRESHOLD;
    
private:

    // The original, undeformed mesh.
    Mesh original_mesh;
    // The resulting skinned (deformed) mesh.
    Mesh skinned_mesh;
    // The skinning data including weights and skinning matrices.
    SkinningData skin_data;
    // The skeleton in bind (reference) pose.
    Skeleton bind_pose;
    // The skeleton in the new pose to deform toward.
    Skeleton new_pose;

    // Performance tracking
    std::unordered_map<std::string, double> timing_metrics;
};
