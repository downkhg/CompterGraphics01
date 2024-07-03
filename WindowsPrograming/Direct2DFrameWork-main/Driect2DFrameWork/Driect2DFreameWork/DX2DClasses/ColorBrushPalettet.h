#pragma once
#include <vector>

using namespace std;

struct ID2D1HwndRenderTarget;
struct ID2D1SolidColorBrush;

namespace DX2DClasses
{
	class CColorBrushPalettet
	{
		static CColorBrushPalettet* m_pColorBrushPalettet;
	public:
		static CColorBrushPalettet* GetInstance() { return m_pColorBrushPalettet; }
	public:
		enum E_BRUSH_COLOR { RED, GREEN, BLUE, YELLOW, WHITE, BLACK, MAX };
	protected:
		vector<class CColorBrush*> m_ColorBlushs;
		void _CreateBrush(ID2D1HwndRenderTarget* pRenderTarget, E_BRUSH_COLOR brushColor);
	public:
		void Initialize(ID2D1HwndRenderTarget* pRenderTarget);
		void Release();

		CColorBrush* GetBrushClass(E_BRUSH_COLOR color);
		ID2D1SolidColorBrush* GetBrush(E_BRUSH_COLOR color);
	};
}