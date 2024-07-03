#pragma once
#include <Windows.h>
/* 
Ű����
[ 0x8000 ] : �۵��� ������ �ִ�.
[ 0x0001 ] : �� ���� ��������.
[ 0x8001 ] : �� ��� �� �߻� ��.
[ 0x0000 ] : �� ��� �� �߻� ����.
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