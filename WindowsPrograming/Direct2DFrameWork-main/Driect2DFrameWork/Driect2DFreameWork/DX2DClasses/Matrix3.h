#pragma once
//https://github.com/choisb/Study-GameProgCpp/blob/master/Ch05_OpenGL/Game/Math.h
namespace DX2DClasses
{
    struct SVector2;

    struct Matrix3
    {
        float mat[3][3];

        Matrix3();

        explicit Matrix3(float inMat[3][3]);

        // Cast to a const float pointer
        const float* GetAsFloatPtr() const;

        // Matrix multiplication
        Matrix3 operator*(const Matrix3& right);

        Matrix3& operator*=(const Matrix3& right);

        // Create a scale matrix with x and y scales
        static Matrix3 CreateScale(float xScale, float yScale);

        static Matrix3 CreateScale(const SVector2& scaleVector);

        // Create a scale matrix with a uniform factor
        static Matrix3 CreateScale(float scale);

        // Create a rotation matrix about the Z axis
        // theta is in radians
        static Matrix3 CreateRotation(float theta);

        // Create a translation matrix (on the xy-plane)
        static Matrix3 CreateTranslation(const SVector2& trans);

        static const Matrix3 Identity;

        const char* GetChar(const char* msg);
    };
}