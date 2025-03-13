#include "math_facade.h"

// Third-party imports
#define HANDMADE_MATH_IMPLEMENTATION
#include "handmade_math/handmade_math.h"


HMM_Mat4 MathFacade::create_identity()
{
    return HMM_M4D(1.0f);
}

HMM_Mat4 MathFacade::create_orthographic(float left, float right, float bottom,
                                         float top, float nearZ, float farZ)
{
    return HMM_Orthographic_RH_NO(left, right, bottom, top, nearZ, farZ);
}

HMM_Mat4 MathFacade::multiply(const HMM_Mat4& m1, const HMM_Mat4& m2)
{
    return HMM_Mul(m1, m2);
}

HMM_Vec3 MathFacade::transform_vec3(const HMM_Mat4& matrix, const HMM_Vec3& vec)
{
    // HandmadeMath provides HMM_MulMat4ByVec4, but we can wrap it for a Vec3.
    // Convert Vec3 to Vec4, multiply, then convert back to Vec3.
    HMM_Vec4 temp = { vec.X, vec.Y, vec.Z, 1.0f };
    temp = HMM_Mul(matrix, temp);
    // Return the (x,y,z) portion
    return HMM_Vec3{temp.X, temp.Y, temp.Z};
}

HMM_Mat4 MathFacade::rotateX(float radians)
{
    return HMM_Rotate_RH(radians, {1.0f, 0.0f, 0.0f});
}

HMM_Mat4 MathFacade::rotateY(float radians)
{
    return HMM_Rotate_RH(radians, {0.0f, 1.0f, 0.0f});
}

HMM_Mat4 MathFacade::rotateZ(float radians)
{
    return HMM_Rotate_RH(radians, {0.0f, 0.0f, 1.0f});
}

HMM_Mat4 MathFacade::scale(float x, float y, float z)
{
    return HMM_Scale({x, y, z});
}

HMM_Mat4 MathFacade::translate(float x, float y, float z)
{
    return HMM_Translate({x, y, z});
}
