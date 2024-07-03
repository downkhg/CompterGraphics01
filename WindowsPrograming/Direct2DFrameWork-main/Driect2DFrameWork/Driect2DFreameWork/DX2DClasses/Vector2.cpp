#include "Vector2.h"
#include "Matrix3x3.h"
#include "UtilityDefine.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <conio.h>

using namespace DX2DClasses;

SVector2::SVector2(float x, float y) 
{ 
	this->x = x; this->y = y;
}
SVector2::SVector2(D2D1_POINT_2F& pos)
{
	this->x = pos.x; this->y = pos.y;
}
SVector2 SVector2::operator+(const SVector2& vec)
{
	SVector2 vTemp(x + vec.x, y + vec.y);
	return vTemp;
}
SVector2 SVector2::operator-(const SVector2& vec)
{
	SVector2 vTemp(x - vec.x, y - vec.y);
	return vTemp;
}
SVector2 SVector2::operator*(const float dist)
{
	SVector2 vTemp(x * dist, y * dist);
	return vTemp;
}
SVector2 SVector2::operator*(SMatrix3x3& mat)
{
	SVector2 vTemp = *this;
	vTemp = mat * vTemp;
	return vTemp;
}
bool SVector2::operator==(SVector2& vec)
{
	return (this->x == vec.x && this->y == vec.y);
}
//SVector2 operator+(const SVector2& a, const SVector2& b)
//{
//	return SVector2(a.x + b.x, a.y + b.y);
//}
//SVector2 operator-(const SVector2& a, const SVector2& b)
//{
//	return SVector2(a.x - b.x, a.y - b.y);
//}
float SVector2::Magnitude()
{
	float fScala = sqrtf(x * x + y * y);
	return fScala;
}
SVector2 SVector2::Normalize()
{
	float fDist = Magnitude();
	SVector2 vDir(x / fDist, y / fDist);
	return vDir;
}

D2D1_POINT_2F SVector2::ToPoint()
{
	D2D1_POINT_2F sPoint = D2D1::Point2F(x, y);
	return sPoint;
}

const char* SVector2::GetChar(const char* msg)
{
	static char strName[256];
	sprintf_s(strName, sizeof(strName), "%s(%f,%f)\n", msg, x, y);
	return strName;
}

const char* SVector2::GetChar(const D2D1_POINT_2F& pos, const char* msg)
{
	static char strName[256];
	sprintf_s(strName, sizeof(strName), "%s(%f,%f)\n", msg, pos.x, pos.y);
	return strName;
}

float SVector2::Dot(const SVector2& a, const SVector2& b)
{
	return a.x * b.x + a.y * b.y;
}

float SVector2::Cross(const SVector2& a, const SVector2& b)
{
	return a.x * b.y - a.y * b.x;
}

SVector2 SVector2::CrossZ(const SVector2& dir)
{
	return SVector2(dir.y, -dir.x);
}

//SVector2 SVector2::CrossZ(const SVector2& a)
//{
//	//임의의 z축 b를 생성하고, 3차원에 외적을 연산한다.
//	//여기서 결과가 유효함을 확인하고, 나머지 필요없는 연산을 걷어낸다.
//	SVector2 b;
//	float fbZ = 1;
//	SVector2 temp;
//	float faZ = 0;
//	//faZ는 0이므로 연산이 필요없는 결과
//	temp.x = a.y;// *fbZ;// -faZ * b.y;
//	temp.y = /*faZ * b.y*/ -a.x;// *fbZ; //z가1이므로 곱하는의미가없음.
//	//결과적으로 a,y, -a.x가 남으므로 다음이 z축과의 수직인 벡터이다.
//	//float ftZ = a.x * b.y - a.y* b.x;
//	return temp;
//}

float SVector2::Angle(const SVector2& a, const SVector2& b)
{
	int fTheta = Dot(a, b);
	return acosf(fTheta) * RAD2DEG;
}

float SVector2::Distance(SVector2& a, const SVector2& b)
{
	SVector2 vDist = a - b;
	return vDist.Magnitude();
}

SVector2 SVector2::RectCenter(SVector2& tl, SVector2& br)
{
	SVector2 vDist = br - tl;
	return tl + vDist * 0.5f;
}

SVector2 SVector2::RectTR(SVector2& tl, SVector2& br)
{
	return SVector2(tl.x, br.y);
}

SVector2 SVector2::RectBL(SVector2& tl, SVector2& br)
{
	return SVector2(br.x, tl.y);
}

void SVector2::TestMain()
{
	const float fRad2Deg = 57.29578f;
	float fDot= SVector2::Dot(SVector2::up(), SVector2::right());
	float fCosA = acosf(fDot);
	float fCosAtoRad = fCosA * fRad2Deg;
	float fAngle = SVector2::Angle(SVector2::up(), SVector2::right());
	SVector2 vTan = SVector2::up() + SVector2::right();
	vTan.Normalize();
	float fTanA = vTan.y / vTan.x;
	float fTanAToRad = atanf(fTanA) * fRad2Deg;
	float fTanA2ToRad = atan2(vTan.y, vTan.x) * fRad2Deg;

	_cprintf("dot: %f / cosA: %f / cosAtoRad: %f \n", fDot, fCosA, fCosAtoRad);
	_cprintf("vTan %s / tanA: %f / tanAtoRad1/2: %f/%f \n", vTan.GetChar(), fTanA, fTanAToRad, fTanA2ToRad);
	_cprintf("Angle: %f \n", fAngle);

	//디버그 라인은 해당 헤더를 가져와야하므로 주석처리함.
	//드로우에 가져다가 사용하며 테스트해봐야함.
	/*CColorBrush* pRedBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::RED);
	CColorBrush* pGreenBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::GREEN);
	CColorBrush* pYellowBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::YELLOW);
	CColorBrush* pBlackBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::BLACK);*/
	SVector2 vRight(1, 0);
	SVector2 vUp(0, 1);
	SVector2 vRU(1, 1);
	vRU = vRU.Normalize();
	SVector2 vLineStart(100, 100);
	SVector2 vLineEnd;
	float fLineDist = 100;
	vLineEnd = vLineStart + vRight * fLineDist;
	//CDebugHelper::DrawLine(vLineStart, vLineEnd, pRedBrush);
	vLineEnd = vLineStart + vUp * fLineDist;
	//CDebugHelper::DrawLine(vLineStart, vLineEnd, pGreenBrush);
	SVector2 vCrossZRight = SVector2::CrossZ(vRight);
	vLineEnd = vLineStart + vCrossZRight * fLineDist;
	//CDebugHelper::DrawLine(vLineStart, vLineEnd, pYellowBrush);
	SVector2 vCrossZUp = SVector2::CrossZ(vUp);
	vLineEnd = vLineStart + vCrossZUp * fLineDist;
	//CDebugHelper::DrawLine(vLineStart, vLineEnd, pBlackBrush);
	vLineEnd = vLineStart + vRU * fLineDist;
	//CDebugHelper::DrawLine(vLineStart, vLineEnd, pRedBrush);
	SVector2 vCrossZRU = SVector2::CrossZ(vRU);
	vLineEnd = vLineStart + vCrossZRU * fLineDist;
	//CDebugHelper::DrawLine(vLineStart, vLineEnd, pYellowBrush);
}

float CosAtoAngle(float fDot)
{
	const float fRad2Deg = 57.29578f;
	return acosf(fDot) * fRad2Deg;
}

float TanAtoAngle(const SVector2& vec)
{
	const float fRad2Deg = 57.29578f;
	float fTanA = vec.y / vec.x;
	return atanf(fTanA) * fRad2Deg;
}

//SVector2 SVector2::Cross(SVector2 a, SVector2 b)
//{
//	/*
//	 temp.x = vector_a.y * vector_b.z - vector_a.z * vector_b.y;
//   temp.y = vector_a.z * vector_b.x - vector_a.x * vector_b.z;
//   temp.z = vector_a.x * vector_b.y - vector_a.y * vector_b.x;
//	*/
//
//	SVector2 vTemp;
//	//float fAz;
//	//float fBz;
//	//float fTempZ;
//
//	vTemp.x = a.y * fBz - fAz * b.y;
//	vTemp.y = a.x * fBz - fAz * b.x;
//	fTempZ = a.x * b.y - a.y * b.x;
//
//
//	return vTemp;
//}