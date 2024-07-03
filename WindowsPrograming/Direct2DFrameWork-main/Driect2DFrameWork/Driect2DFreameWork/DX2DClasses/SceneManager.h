#pragma once
#include <Windows.h>

namespace DX2DClasses
{
	class CDriect2DFramwork;

	__interface ISceneManager
	{
	public:
		virtual void Initialize(HWND hWnd, CDriect2DFramwork* pDX2DFramework) ; //초기화
		virtual void Release() = 0; //동적객체 제거

		virtual void Update() = 0; //정보변경
		virtual void Draw() = 0; //객체 그리기
	};
}