#include "test_utils.h"

// Standard library imports
#include <cmath>
#include <vector>

// Third-party imports
#include "handmade_math/handmade_math.h"

// Local application imports
#include "model/mesh.h"


bool approx_equal(float a, float b, float epsilon/*= .0001f*/) 
{
    return std::fabs(a - b) < epsilon;
}

bool approx_equal_vec3(const HMM_Vec3& a, const HMM_Vec3& b, float epsilon/*= .0001f*/) 
{
    return approx_equal(a.X, b.X, epsilon) && 
           approx_equal(a.Y, b.Y, epsilon) && 
           approx_equal(a.Z, b.Z, epsilon);
}

bool approx_equal_mat4(const HMM_Mat4& a, const HMM_Mat4& b, float epsilon/*= .0001f*/)
{
    for (size_t row = 0; row < 4; row++) 
    {
        for (size_t col = 0; col < 4; col++) 
        {
            if (!approx_equal(a.Elements[row][col], b.Elements[row][col], epsilon)) 
            {
                return false;
            }
        }
    }

    return true;
}

bool is_vertex_connected(const std::vector<Face>& faces, size_t vertex_index)
{
    for (const auto& face : faces)
    {
        for (const auto& index : face.indices)
        {
            if (index == vertex_index)
            {
                return true;
            }
        }
    }

    return false;
}

std::pair<HMM_Vec3, HMM_Vec3> calculate_mesh_bounds(const std::vector<HMM_Vec3>& vertices)
{
    if (vertices.empty())
    {
        return {HMM_V3(0.f, 0.f, 0.f), HMM_V3(0.f, 0.f, 0.f)};
    }
    
    HMM_Vec3 min_bounds = vertices[0];
    HMM_Vec3 max_bounds = vertices[0];
    
    for (const auto& vertex : vertices)
    {
        min_bounds.X = std::min(min_bounds.X, vertex.X);
        min_bounds.Y = std::min(min_bounds.Y, vertex.Y);
        min_bounds.Z = std::min(min_bounds.Z, vertex.Z);
        
        max_bounds.X = std::max(max_bounds.X, vertex.X);
        max_bounds.Y = std::max(max_bounds.Y, vertex.Y);
        max_bounds.Z = std::max(max_bounds.Z, vertex.Z);
    }
    
    return {min_bounds, max_bounds};
}
