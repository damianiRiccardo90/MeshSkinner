#pragma once

// Third-party imports
#include "handmade_math/handmade_math.h"


/**
 * @brief A Facade class to simplify interactions with the HandmadeMath library.
 */
class MathFacade
{
public:

    /**
     * @brief Creates an identity matrix.
     * @return An HMM_Mat4 representing the identity matrix.
     */
    static HMM_Mat4 create_identity();

    /**
     * @brief Creates an orthographic projection matrix.
     * @param left   Left coordinate of the clipping plane.
     * @param right  Right coordinate of the clipping plane.
     * @param bottom Bottom coordinate of the clipping plane.
     * @param top    Top coordinate of the clipping plane.
     * @param nearZ  Near clipping plane.
     * @param farZ   Far clipping plane.
     * @return The resultant orthographic projection matrix.
     */
    static HMM_Mat4 create_orthographic(float left, float right, float bottom,
                                        float top, float nearZ, float farZ);
    
    /**
     * @brief Computes the inverse of a 4x4 matrix.
     * @param matrix The matrix to invert.
     * @return The inverse of the input matrix.
     */
    static HMM_Mat4 inverse(const HMM_Mat4& matrix);

    /**
     * @brief Multiplies two 4x4 matrices.
     * @param m1 First matrix.
     * @param m2 Second matrix.
     * @return The product matrix (m1 * m2).
     */
    static HMM_Mat4 multiply(const HMM_Mat4& m1, const HMM_Mat4& m2);

    /**
     * @brief Transforms a 3D vector using a 4x4 matrix.
     * @param matrix The 4x4 transformation matrix.
     * @param vec    The 3D vector to transform.
     * @return A transformed 3D vector.
     */
    static HMM_Vec3 transform_vec3(const HMM_Mat4& matrix, const HMM_Vec3& vec);

    /**
     * @brief Creates a rotation around the X axis.
     * @param radians Angle in radians.
     * @return The rotation matrix.
     */
    static HMM_Mat4 rotateX(float radians);

    /**
     * @brief Creates a rotation around the Y axis.
     * @param radians Angle in radians.
     * @return The rotation matrix.
     */
    static HMM_Mat4 rotateY(float radians);

    /**
     * @brief Creates a rotation around the Z axis.
     * @param radians Angle in radians.
     * @return The rotation matrix.
     */
    static HMM_Mat4 rotateZ(float radians);

    /**
     * @brief Creates a scaling matrix.
     * @param x Scale along the x-axis.
     * @param y Scale along the y-axis.
     * @param z Scale along the z-axis.
     * @return The scaling matrix.
     */
    static HMM_Mat4 scale(float x, float y, float z);

    /**
     * @brief Creates a translation matrix for the given x, y, z coordinates
     * @param x Amount to translate along the x-axis.
     * @param y Amount to translate along the y-axis.
     * @param z Amount to translate along the z-axis.
     * @return The translation matrix.
     */
    static HMM_Mat4 translate(float x, float y, float z);

private:

    // Private constructor to discourage instantiation;
    // our methods are all static.
    MathFacade() = default;
};
