#include "skinning_data.h"

// Standard library imports
#include <algorithm>
#include <stdexcept>

// Local application imports
#include "facade/json_facade.h"
#include "facade/math_facade.h"


std::vector<VertexWeights> SkinningData::parse_weights_from_json(const Json& json_obj)
{
    // Reserve space for all vertices to avoid reallocations
    std::vector<VertexWeights> result;
    result.reserve(json_obj.size());

    // Parse each vertex's bone weights
    for (size_t vertex_idx = 0; vertex_idx < json_obj.size(); vertex_idx++)
    {
        // Initialize a new VertexWeights structure with zeros
        VertexWeights vertex_weights;
        std::fill_n(vertex_weights.joint_ids, VertexWeights::MAX_INFLUENCES, 0);
        std::fill_n(vertex_weights.weights, VertexWeights::MAX_INFLUENCES, 0.0f);
        
        // Get the JSON object for this vertex
        const Json& vertex_data = json_obj.at(vertex_idx);
        
        // Ensure the JSON has the required fields
        if (!vertex_data.contains("weight") || !vertex_data.contains("index"))
        {
            throw std::runtime_error("Vertex weight data missing required fields");
        }
        
        const Json& weights_json = vertex_data["weight"];
        const Json& indices_json = vertex_data["index"];
        
        // Determine how many influences to process (limited by our fixed array size)
        const size_t num_influences = std::min(
            std::min(weights_json.size(), indices_json.size()), 
            VertexWeights::MAX_INFLUENCES
        );
        
        // Copy the weight and joint ID data for each influence
        for (size_t influence_idx = 0; influence_idx < num_influences; influence_idx++)
        {
            // Read weight value - defaults to 0 if conversion fails
            vertex_weights.weights[influence_idx] = weights_json.at(influence_idx).as_float();
            
            // Read joint ID - defaults to 0 if conversion fails
            vertex_weights.joint_ids[influence_idx] = indices_json.at(influence_idx).as_int();
        }
        
        // Add this vertex's weights to our result vector
        result.push_back(vertex_weights);
    }

    return result;
}

std::vector<HMM_Mat4> SkinningData::parse_matrices_from_json(const Json& json_obj)
{
    // Reserve space for all matrices to avoid reallocations
    std::vector<HMM_Mat4> matrices;
    matrices.reserve(json_obj.size());
    
    // Parse each matrix from the JSON array
    for (size_t matrix_idx = 0; matrix_idx < json_obj.size(); matrix_idx++)
    {
        const Json& matrix_data = json_obj.at(matrix_idx);
        
        // Each matrix must have exactly 16 elements (4x4 matrix)
        if (matrix_data.size() != 16)
        {
            throw std::runtime_error("Matrix JSON must contain exactly 16 elements");
        }
        
        // Start with identity matrix
        HMM_Mat4 matrix = MathFacade::create_identity();
        
        // Fill the matrix with values from JSON
        // The JSON array is a flat list of 16 elements in row-major order
        for (size_t elem_idx = 0; elem_idx < 16; elem_idx++)
        {
            size_t row = elem_idx / 4;  // Integer division gives row index
            size_t col = elem_idx % 4;  // Modulo gives column index
            matrix.Elements[row][col] = matrix_data.at(elem_idx).as_float();
        }
        
        // Add this matrix to our result vector
        matrices.push_back(matrix);
    }
    
    return matrices;
}

HMM_Mat4 SkinningData::get_skinning_matrix(int joint_id) const
{
    // Safety check: ensure joint ID is valid (non-negative and within bounds)
    if (joint_id < 0 || 
        static_cast<size_t>(joint_id) >= pose_matrices.size() || 
        static_cast<size_t>(joint_id) >= inverse_bind_matrices.size())
    {
        throw std::out_of_range("Joint ID out of range");
    }
    
    // The skinning matrix combines the pose transformation with the inverse bind pose
    // This gives us the transformation from bind pose space to current pose space
    return MathFacade::multiply(pose_matrices[joint_id], inverse_bind_matrices[joint_id]);
}
