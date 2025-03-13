#pragma once

// Third-party imports
#include "handmade_math/handmade_math.h"


// Comparison helper for floating point values
bool approx_equal(float a, float b, float epsilon = .0001f);

// Comparison helper for vectors using handmade math
bool approx_equal_vec3(const HMM_Vec3& a, const HMM_Vec3& b, float epsilon = .0001f);
