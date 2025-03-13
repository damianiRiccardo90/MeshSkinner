#include "test_utils.h"

// Standard library imports
#include <cmath>
#include <vector>


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
