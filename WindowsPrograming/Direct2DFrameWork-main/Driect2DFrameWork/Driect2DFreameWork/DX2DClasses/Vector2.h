#pragma once
#include <d2d1_1helper.h>

namespace DX2DClasses
{
	struct SMatrix3x3;

	struct SVector2
	{
		float x;
		float y;

		SVector2(float x = 0, float y = 0);
		SVector2(D2D1_POINT_2F& pos);
		SVector2 operator+(const SVector2& vec);
		SVector2 operator-(const SVector2& vec);
		SVector2 operator*(const float dist);
		SVector2 operator*(SMatrix3x3& mat);
		bool operator==(SVector2& vec);
		//friend SVector2 operator+(const SVector2& a, const SVector2& b);
		//friend SVector2 operator-(const SVector2& a, const SVector2& b);
		float Magnitude();
		SVector2 Normalize();
		D2D1_POINT_2F ToPoint();

		const char* GetChar(const char* msg =  "");//정적지역변수사용하므로 같은줄에 두번호출하면 결과가 정상적이지않음. 참고할것.
		static const char* GetChar(const D2D1_POINT_2F& pos, const char* msg = "");//정적지역변수사용하므로 같은줄에 두번호출하면 결과가 정상적이지않음. 참고할것.

		static float Dot(const SVector2& a, const SVector2& b);
		static float Cross(const SVector2& a, const SVector2& b);//2D공간상에서 z축은 없으므로 계산결과 z축의 스칼라만 남는다.//출처:https://allenchou.net/2013/07/cross-product-of-2d-vectors/
		static SVector2 CrossZ(const SVector2& dir);//2D선분의 직각이되는 선분을 구할때 사용할것.
		static float Angle(const SVector2& a, const SVector2& b);
		static float Distance(SVector2& a, const SVector2& b);

		static SVector2 RectCenter(SVector2& tl, SVector2& br);
		static SVector2 RectTR(SVector2& tl, SVector2& br);
		static SVector2 RectBL(SVector2& tl, SVector2& br);

		static SVector2& up() { static SVector2 up(0, -1); return up; };
		static SVector2& right() { static SVector2 right(1, 0); return right; }
		static SVector2& down() { static SVector2 down(0, 1); return down; }
		static SVector2& left() { static SVector2 left(-1, 0); return left; }

		static void TestMain();
	};
}