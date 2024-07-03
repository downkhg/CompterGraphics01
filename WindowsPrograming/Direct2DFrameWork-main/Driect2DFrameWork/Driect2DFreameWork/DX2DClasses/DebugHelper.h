#pragma once
#include <typeinfo>

namespace D2D1
{
	class Matrix3x2F;
}

namespace DX2DClasses
{
	class CGameObject;
	class CColorBrush;
	struct SVector2;
	class D2D1::Matrix3x2F;

	class CDebugHelper
	{
	public: 
		//�ֵܼ����
		static void OpenConsole();
		static void CloseConsole();
		static void LogConsole(const char* msg, ...);
	public: 
		//����� ������������. ���� ���� �ʿ�.
		static void DrawLine(SVector2& start, SVector2& end, CColorBrush* pColorBrush, float stroke = 1.0f);
		static void DrawCircle(SVector2& pos, float rad, CColorBrush* pColorBrush, float stroke = 1.0f);
		static void DrawRect(SVector2& tl, SVector2& br, CColorBrush* pColorBrush, float stroke = 1.0f);
		static void DrawPolygon(SVector2 posArr[], int size, CColorBrush* pColorBrush, float stroke = 1.0f);

		//�׷��ȵ����
		static void DrawLine(D2D1::Matrix3x2F& mat, SVector2& start, SVector2& end, CColorBrush* pColorBrush, float stroke = 1.0f);
		static void DrawCircle(D2D1::Matrix3x2F& mat, SVector2& pos, float rad, CColorBrush* pColorBrush, float stroke = 1.0f);
		static void DrawRect(D2D1::Matrix3x2F& mat, SVector2& tl, SVector2& br, CColorBrush* pColorBrush, float stroke = 1.0f);
		static void DrawPolygon(D2D1::Matrix3x2F& mat, SVector2 posArr[], int size, CColorBrush* pColorBrush, float stroke = 1.0f);
		
		static void DrawCircle(CGameObject* pGameObject, CColorBrush* pColorBrush, float stroke = 1.0f);
		static void DrawRect(CGameObject* pGameObject, CColorBrush* pColorBrush, float stroke = 1.0f);
		static void DrawBox(CGameObject* pGameObject, CColorBrush* pColorBrush, float stroke = 1.0f);
	};
}