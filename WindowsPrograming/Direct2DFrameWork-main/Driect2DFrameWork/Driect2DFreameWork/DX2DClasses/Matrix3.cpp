#include "Matrix3.h"
#include "Vector2.h"
#include <math.h>
#include<string>

using namespace DX2DClasses;

static float m3Ident[3][3] =
{
    { 1.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f }
};
const Matrix3 Matrix3::Identity(m3Ident);

Matrix3::Matrix3()
{
    *this = Matrix3::Identity;
}

Matrix3::Matrix3(float inMat[3][3])
{
    memcpy(mat, inMat, 9 * sizeof(float));
}

// Cast to a const float pointer
const float* Matrix3::GetAsFloatPtr() const
{
    return reinterpret_cast<const float*>(&mat[0][0]);
}

// Matrix multiplication
Matrix3 Matrix3::operator*(const Matrix3& right)
{
    Matrix3 retVal;
    // row 0
    retVal.mat[0][0] =
        mat[0][0] * right.mat[0][0] +
        mat[0][1] * right.mat[1][0] +
        mat[0][2] * right.mat[2][0];

    retVal.mat[0][1] =
        mat[0][0] * right.mat[0][1] +
        mat[0][1] * right.mat[1][1] +
        mat[0][2] * right.mat[2][1];

    retVal.mat[0][2] =
        mat[0][0] * right.mat[0][2] +
        mat[0][1] * right.mat[1][2] +
        mat[0][2] * right.mat[2][2];

    // row 1
    retVal.mat[1][0] =
        mat[1][0] * right.mat[0][0] +
        mat[1][1] * right.mat[1][0] +
        mat[1][2] * right.mat[2][0];

    retVal.mat[1][1] =
        mat[1][0] * right.mat[0][1] +
        mat[1][1] * right.mat[1][1] +
        mat[1][2] * right.mat[2][1];

    retVal.mat[1][2] =
        mat[1][0] * right.mat[0][2] +
        mat[1][1] * right.mat[1][2] +
        mat[1][2] * right.mat[2][2];

    // row 2
    retVal.mat[2][0] =
        mat[2][0] * right.mat[0][0] +
        mat[2][1] * right.mat[1][0] +
        mat[2][2] * right.mat[2][0];

    retVal.mat[2][1] =
        mat[2][0] * right.mat[0][1] +
        mat[2][1] * right.mat[1][1] +
        mat[2][2] * right.mat[2][1];

    retVal.mat[2][2] =
        mat[2][0] * right.mat[0][2] +
        mat[2][1] * right.mat[1][2] +
        mat[2][2] * right.mat[2][2];

    return retVal;
}

Matrix3& Matrix3::operator*=(const Matrix3& right)
{
    Matrix3 mat = *this;
    mat = mat * right;
    return mat;
}

// Create a scale matrix with x and y scales
Matrix3 Matrix3::CreateScale(float xScale, float yScale)
{
    float temp[3][3] =
    {
        { xScale, 0.0f, 0.0f },
        { 0.0f, yScale, 0.0f },
        { 0.0f, 0.0f, 1.0f },
    };
    return Matrix3(temp);
}

Matrix3 Matrix3::CreateScale(const SVector2& scaleVector)
{
    return CreateScale(scaleVector.x, scaleVector.y);
}

// Create a scale matrix with a uniform factor
Matrix3 Matrix3::CreateScale(float scale)
{
    return CreateScale(scale, scale);
}

// Create a rotation matrix about the Z axis
// theta is in radians
Matrix3 Matrix3::CreateRotation(float theta)
{
    float temp[3][3] =
    {
        { cosf(theta), sinf(theta), 0.0f },
        { -sinf(theta), cosf(theta), 0.0f },
        { 0.0f, 0.0f, 1.0f },
    };
    return Matrix3(temp);
}

// Create a translation matrix (on the xy-plane)
Matrix3 Matrix3::CreateTranslation(const SVector2& trans)
{
    float temp[3][3] =
    {
        { 1.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { trans.x, trans.y, 1.0f },
    };
    return Matrix3(temp);
}

const char* Matrix3::GetChar(const char* msg)
{
    static char strName[256];
    sprintf_s(strName, sizeof(strName),
        "#### %s ####\n(%f,%f,%f)\n(%f,%f,%f)\n(%f,%f,%f)\n",
        msg, mat[0][0], mat[0][1], mat[0][2], mat[1][0], mat[1][1], mat[1][2], mat[2][0], mat[2][1], mat[2][2]);
    return strName;
}