#pragma once
#include <d2d1helper.h>

struct ID2D1HwndRenderTarget;
struct ID2D1SolidColorBrush;

namespace DX2DClasses
{
	class CColorBrush
	{
		ID2D1SolidColorBrush* m_pColorBrush;
	public:
		CColorBrush(ID2D1SolidColorBrush* pColorBrush = NULL);
		void CreateColorBrush(ID2D1HwndRenderTarget* pRenderTarget, const D2D1_COLOR_F& color);
		void SetColorBrush(ID2D1SolidColorBrush* pColorBrush);
		ID2D1SolidColorBrush* GetColorBrush();
	};
}