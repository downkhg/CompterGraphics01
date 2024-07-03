#include "Matrix3x3.h"
#include "Vector2.h"
#include "UtilityDefine.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "DebugHelper.h"

//https://webgl2fundamentals.org/webgl/lessons/ko/webgl-2d-matrices.html
using namespace DX2DClasses;

void SMatrix3x3::InitIdentity()
{
	memset(m, 0, sizeof(m));
	m11 = 1; m22 = 1; m33 = 1;
}

SMatrix3x3::SMatrix3x3()
{
	InitIdentity();
}

SMatrix3x3::SMatrix3x3(const D2D1::Matrix3x2F& mat)
{
	InitIdentity();
	m11 = mat._11; m12 = mat._12;
	m11 = mat._21; m12 = mat._22;
	m11 = mat._31; m12 = mat._32;
}

SMatrix3x3 SMatrix3x3::operator*(SMatrix3x3& mat)
{
	//https://github.com/choisb/Study-GameProgCpp/blob/master/Ch05_OpenGL/Game/Math.h
	SMatrix3x3 matResult;
	//
	//matResult.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0];

	//matResult.m[0][1] = m[0][0] * mat.m[0][1] + m[0][1] * mat.m[1][1] + m[0][2] * mat.m[2][1];

	//matResult.m[0][2] = m[0][0] * mat.m[0][2] + m[0][1] * mat.m[1][2] +m[0][2] * mat.m[2][2];
	////
	//matResult.m[1][0] =
	//	m[1][0] * mat.m[0][0] +
	//	m[1][1] * mat.m[1][0] +
	//	m[1][2] * mat.m[2][0];

	//matResult.m[1][1] =
	//	m[1][0] * mat.m[0][1] +
	//	m[1][1] * mat.m[1][1] +
	//	m[1][2] * mat.m[2][1];

	//matResult.m[1][2] =
	//	m[1][0] * mat.m[0][2] +
	//	m[1][1] * mat.m[1][2] +
	//	m[1][2] * mat.m[2][2];
	////
	//matResult.m[2][0] =
	//	m[2][0] * mat.m[0][0] +
	//	m[2][1] * mat.m[1][0] +
	//	m[2][2] * mat.m[2][0];

	//matResult.m[2][1] =
	//	m[2][0] * mat.m[0][1] +
	//	m[2][1] * mat.m[1][1] +
	//	m[2][2] * mat.m[2][1];

	//matResult.m[2][2] =
	//	m[2][0] * mat.m[0][2] +
	//	m[2][1] * mat.m[1][2] +
	//	m[2][2] * mat.m[2][2];


	//CDebugHelper::LogConsole("###################\n");
	/*for (int y = 0; y < 3; y++) 
		for (int x = 0; x < 3; x++) 
			for (int k = 0; k < 3; k++)
			{
				matResult.m[y][x] = m[y][k] * mat.m[k][x];
				CDebugHelper::LogConsole("[%d][%d] = [%d][%d] * [%d][%d]\n", y, x, y, k, k, x);
			}*/
	//for (int y = 0; y < 3; y++)
	//	for (int x = 0; x < 3; x++)
	//		//for (int k = 0; k < 3; k++)
	//		{
	//			//matResult.m[0][0] = m[0][0] * mat.m[0][0] + m[0][1] * mat.m[1][0] + m[0][2] * mat.m[2][0];
	//			matResult.m[y][x] = m[y][0] * mat.m[0][x] + m[y][1] * mat.m[1][x] + m[y][2] * mat.m[2][x];
	//			//CDebugHelper::LogConsole("[%d][%d] = [%d][%d] * [%d][%d]\n", y, x, y, k, k, x);
	//		}
	//검산결과 위공식과 루프가 다른점을 찾아내고 고침.
	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 3; x++)
		{
			CDebugHelper::LogConsole("[%d][%d] = ", y, x);
			double sum = 0;
			for (int k = 0; k < 3; k++)
			{
				sum += (double)m[y][k] * (double)mat.m[k][x];
				CDebugHelper::LogConsole(" [%d][%d] * [%d][%d] +", y, k, k ,x);
			}
			matResult.m[y][x] = sum;
			CDebugHelper::LogConsole("\n");
		}
	//CDebugHelper::LogConsole("###################\n");
	/*m31 += mat.m31;
	m32 += mat.m32;*/

	return matResult;
}
SVector2 SMatrix3x3::operator*(SVector2& vec)
{
	float w = 1; //Vector2는 3x3행렬과 계산이 불가능하므로 3번째원소 허수 w를 정의한다.
	SVector2 vResult;
	vResult.x = vec.x * m11 + vec.y * m21 + w * m31;
	vResult.y = vec.x * m12 + vec.y * m22 + w * m32;
	////아무리 찾아봐도 공식이 안나와서 다음 코드 참조함. 정상작동확인.
	////https://github.com/choisb/Study-GameProgCpp/blob/master/Ch05_OpenGL/Game/Math.h
	//vResult.x = vec.x * m[0][0] + vec.y * m[1][0] + w * m[2][0];
	//vResult.y = vec.x * m[0][1] + vec.y * m[1][1] + w * m[2][1];
	return vResult;
}
bool SMatrix3x3::operator==(const SMatrix3x3& mat)
{
	
	return false;
}

const char* SMatrix3x3::GetChar(const char* msg)
{
	static char strName[256];
	sprintf_s(strName, sizeof(strName), 
		"#### %s ####\n(%f,%f,%f)\n(%f,%f,%f)\n(%f,%f,%f)\n", 
		msg, m11, m12, m13, m21, m22, m23, m31, m32, m33);
	return strName;
}

const char* SMatrix3x3::GetChar(D2D1_MATRIX_3X2_F& mat, const char* msg)
{
	static char strName[256];
	sprintf_s(strName, sizeof(strName),
		"#### %s ####\n(%f,%f,0)\n(%f,%f,0)\n(%f,%f,1)\n",
		msg, mat.m11, mat.m12, mat.m21, mat.m22, mat.dx, mat.dy);
	return strName;
}

void SMatrix3x3::Translation(const float x, const float y)
{
	m31 = x;
	m32 = y;
	/*float temp[3][3] =
	{
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ x, y, 1.0f },
	};
	memcpy_s(m, sizeof(m), temp, sizeof(temp));*/
}
void SMatrix3x3::Translation(const SVector2& pos)
{
	m31 = pos.x;
	m32 = pos.y;
	/*float temp[3][3] =
	{
		{ 1.0f, 0.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ pos.x, pos.y, 1.0f },
	};
	memcpy_s(m, sizeof(m), temp, sizeof(temp));*/
}
void SMatrix3x3::Rotate(const float fAngle)
{
	double theta = fAngle * DEG2RAD;
	float s = sinf(theta);
	float c = cosf(theta);
	//https://webgl2fundamentals.org/webgl/lessons/ko/webgl-2d-matrices.html
	//결과가 일치하지않아 D2D1에 결과와 비교했으나 일치하지않음.
	/*m11 = c;
	m12 = -s; 
	m21 = s; 
	m22 = c;*/

	//https://github.com/choisb/Study-GameProgCpp/blob/master/Ch05_OpenGL/Game/Math.h
	//D2D1의 함수와 결과가 같음을 확인함. Matrix3함수 참조.
	/*float temp[3][3] =
	{
		{ c, s, 0.0f },
		{ -s, c, 0.0f },
		{ 0.0f, 0.0f, 1.0f },
	};
	memcpy_s(m, sizeof(m), temp, sizeof(temp));*/
	m11 = c; m12 = s;
	m21 = -s; m22 = c;
}
void SMatrix3x3::Rotate(const float fAngle, SVector2& vCenter )
{
	//http://egloos.zum.com/scripter/v/1618411
	double theta = fAngle * DEG2RAD;
	float s = sinf(theta);
	float c = cosf(theta);
	
	//float a
	//x = (x-vCenter.x) * c - (y -vCenter.y)s
	//y = (x-vCenter.x) * s + (y- vCenter.y)c


	m11 = c; m12 = s;
	m21 = -s; m22 = c;
}
void SMatrix3x3::Scale(const SVector2& size)
{
	m11 = size.x;
	m22 = size.y;
}
void SMatrix3x3::Scale(float x, float y)
{
	m11 = x;
	m22 = y;
}

D2D1::Matrix3x2F SMatrix3x3::ToMatrix3x2()
{
	D2D1::Matrix3x2F matResult;
	matResult._11 = this->m11;
	matResult._12 = this->m12;
	matResult._21 = this->m21;
	matResult._22 = this->m22;
	matResult._31 = this->m31;
	matResult._32 = this->m32;
	return matResult;
}