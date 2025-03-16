#pragma once

// Standard library imports
#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

// Third-party imports
#include "handmade_math/handmade_math.h"

// Local application imports
#include "model/mesh.h"
#include "model/skinning_data.h"


/**
 * @brief A class for performing linear blend skinning on 3D meshes.
 *
 * This class manages the skinning process from loading input data (mesh, matrices, weights)
 * to applying the transformations and outputting the deformed mesh.
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
     * @brief Loads inverse bind pose matrices from a JSON file.
     * @param inv_bind_path The path to the inverse bind pose JSON file.
     * @return true if the matrices were loaded successfully; otherwise false.
     */
    bool load_inverse_bind_matrices(const std::string& inv_bind_path);

    /**
     * @brief Loads pose matrices from a JSON file.
     * @param pose_path The path to the pose matrices JSON file.
     * @return true if the matrices were loaded successfully; otherwise false.
     */
    bool load_output_pose_matrices(const std::string& pose_path);
    
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
     * @brief Applies precomputed transformations to each vertex to produce the skinned mesh.
     *
     * @param precomputed_matrices A vector of precomputed skinning matrices for each joint.
     */
    void apply_vertex_transformations(const std::vector<HMM_Mat4>& precomputed_matrices);

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

    // Performance tracking
    std::unordered_map<std::string, double> timing_metrics;
};