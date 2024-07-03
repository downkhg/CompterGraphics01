#pragma once
#include <Windows.h>

namespace DX2DClasses
{
	class CDriect2DFramwork;

	__interface ISceneManager
	{
	public:
		virtual void Initialize(HWND hWnd, CDriect2DFramwork* pDX2DFramework) ; //�ʱ�ȭ
		virtual void Release() = 0; //������ü ����

		virtual void Update() = 0; //��������
		virtual void Draw() = 0; //��ü �׸���
	};
}