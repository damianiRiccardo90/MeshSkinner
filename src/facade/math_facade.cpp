#define HANDMADE_MATH_IMPLEMENTATION

#include "math_facade.h"


HMM_Mat4 MathFacade::create_identity()
{
    return HMM_M4D(1.f);
}

HMM_Mat4 MathFacade::create_orthographic(float left, float right, float bottom,
                                         float top, float nearZ, float farZ)
{
    return HMM_Orthographic_RH_NO(left, right, bottom, top, nearZ, farZ);
}

HMM_Mat4 MathFacade::inverse(const HMM_Mat4& matrix)
{
    return HMM_InvGeneral(matrix);
}

HMM_Mat4 MathFacade::multiply(const HMM_Mat4& m1, const HMM_Mat4& m2)
{
    return HMM_Mul(m1, m2);
}

HMM_Vec3 MathFacade::transform_vec3(const HMM_Mat4& matrix, const HMM_Vec3& vec)
{
    // HandmadeMath provides HMM_MulMat4ByVec4, but we can wrap it for a Vec3.
    // Convert Vec3 to Vec4, multiply, then convert back to Vec3.
    HMM_Vec4 temp = HMM_V4(vec.X, vec.Y, vec.Z, 1.f);
    temp = HMM_MulM4V4(matrix, temp);
    // Return the (x,y,z) portion
    return HMM_V3(temp.X, temp.Y, temp.Z);
}

HMM_Mat4 MathFacade::rotateX(float radians)
{
    return HMM_Rotate_RH(radians, HMM_V3(1.f, 0.f, 0.f));
}

HMM_Mat4 MathFacade::rotateY(float radians)
{
    return HMM_Rotate_RH(radians, HMM_V3(0.f, 1.f, 0.f));
}

HMM_Mat4 MathFacade::rotateZ(float radians)
{
    return HMM_Rotate_RH(radians, HMM_V3(0.f, 0.f, 1.f));
}

HMM_Mat4 MathFacade::scale(float x, float y, float z)
{
    return HMM_Scale(HMM_V3(x, y, z));
}

HMM_Mat4 MathFacade::translate(float x, float y, float z)
{
    return HMM_Translate(HMM_V3(x, y, z));
}
