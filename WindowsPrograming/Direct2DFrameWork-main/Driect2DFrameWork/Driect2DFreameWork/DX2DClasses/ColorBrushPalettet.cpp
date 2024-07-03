#include "ColorBrushPalettet.h"
#include "ColorBrush.h"

using namespace DX2DClasses;

CColorBrushPalettet* CColorBrushPalettet::m_pColorBrushPalettet = NULL;

void CColorBrushPalettet::_CreateBrush(ID2D1HwndRenderTarget* pRenderTarget, E_BRUSH_COLOR brushColor)
{
	CColorBrush* pColorBrush = new CColorBrush();
	D2D1_COLOR_F color;
	switch (brushColor)
	{
	case E_BRUSH_COLOR::RED: color = D2D1::ColorF(D2D1::ColorF::Red); break;
	case E_BRUSH_COLOR::GREEN: color = D2D1::ColorF(D2D1::ColorF::Green); break;
	case E_BRUSH_COLOR::BLUE: color = D2D1::ColorF(D2D1::ColorF::Blue); break;
	case E_BRUSH_COLOR::YELLOW: color = D2D1::ColorF(D2D1::ColorF::Yellow); break;
	case E_BRUSH_COLOR::WHITE: color = D2D1::ColorF(D2D1::ColorF::White); break;
	case E_BRUSH_COLOR::BLACK: color = D2D1::ColorF(D2D1::ColorF::Black); break;
	}
	pColorBrush->CreateColorBrush(pRenderTarget, color);
	m_ColorBlushs[brushColor] = pColorBrush;
}

void CColorBrushPalettet::Initialize(ID2D1HwndRenderTarget* pRenderTarget)
{
	m_ColorBlushs.resize(E_BRUSH_COLOR::MAX);
	for (int color = 0; color < E_BRUSH_COLOR::MAX; color++)
		_CreateBrush(pRenderTarget, (E_BRUSH_COLOR)color);
	m_pColorBrushPalettet = this;
}

void CColorBrushPalettet::Release()
{
	for (int i = 0; i < m_ColorBlushs.size(); i++)
		delete m_ColorBlushs[i];
	m_ColorBlushs.clear();
}

CColorBrush* CColorBrushPalettet::GetBrushClass(E_BRUSH_COLOR color)
{
	return m_ColorBlushs[color];
}

ID2D1SolidColorBrush* CColorBrushPalettet::GetBrush(E_BRUSH_COLOR color)
{
	return m_ColorBlushs[color]->GetColorBrush();
}