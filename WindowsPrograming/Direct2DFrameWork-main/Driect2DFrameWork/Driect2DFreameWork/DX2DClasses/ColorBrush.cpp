#include "ColorBrush.h"

using namespace DX2DClasses;

CColorBrush::CColorBrush(ID2D1SolidColorBrush* pColorBrush)
{ 
	m_pColorBrush = pColorBrush; 
}
void CColorBrush::CreateColorBrush(ID2D1HwndRenderTarget* pRenderTarget, const D2D1_COLOR_F& color)
{
	pRenderTarget->CreateSolidColorBrush(color, &m_pColorBrush);
}
void CColorBrush::SetColorBrush(ID2D1SolidColorBrush* pColorBrush)
{ 
	m_pColorBrush = pColorBrush; 
}
ID2D1SolidColorBrush* CColorBrush::GetColorBrush()
{ 
	return m_pColorBrush; 
}