#pragma once
#include <d2d1_1helper.h>

//https://github.com/choisb/Study-GameProgCpp/blob/master/Ch05_OpenGL/Game/Math.h
namespace DX2DClasses
{
	struct SVector2;

	struct SMatrix3x3
	{
		//공용체를 응용하여 각맴버에 배열과 일반맴버접근 인터페이스를 모두 제공한다.
		union
		{
			struct
			{
				float m11, m12, m13;
				float m21, m22, m23;
				float m31, m32, m33;
			};
			float m[3][3];
		};
		SMatrix3x3();
		SMatrix3x3(const D2D1::Matrix3x2F& mat);

		SMatrix3x3 operator*(SMatrix3x3& mat);
		SVector2 operator*(SVector2& vec);
		bool operator==(const SMatrix3x3& mat);

		const char* GetChar(const char* msg = "");//정적지역변수사용하므로 같은줄에 두번호출하면 결과가 정상적이지않음. 참고할것.
		static const char* GetChar(D2D1_MATRIX_3X2_F& mat, const char* msg = "");

		void InitIdentity();

		void Translation(const float x, const float y);
		void Translation(const SVector2& pos);
		void Rotate(const float fAngle);
		void Rotate(const float fAngle, SVector2& center);
		void Scale(float x, float y);
		void Scale(const SVector2& size);

		D2D1::Matrix3x2F ToMatrix3x2();
	};
}
//#define DEBUG_OVERLAP
