#pragma once
#include <Windows.h>
/* 
키상태
[ 0x8000 ] : 작동시 누르고 있다.
[ 0x0001 ] : 그 전에 눌렀었다.
[ 0x8001 ] : 두 경우 다 발생 함.
[ 0x0000 ] : 두 경우 다 발생 안함.
*/
namespace DX2DClasses
{
	class CInputManager
	{
	public:
		enum E_KEY_STATE { PRESS = 0x8000, RELEASE = 0x001,};

		static bool GetAsyncKeyStatePress(int nKey);
		static bool GetAsyncKeyStateRelease(int nKey);
		static bool GetAsyncKeyStateType(int nKey, E_KEY_STATE eState);
	};
}