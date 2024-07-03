#include "DebugHelper.h"
#include "ColorBrush.h"
#include "Vector2.h"
#include "SingletonRenderTarget.h"
#include "GameObject.h"
#include "Image.h"
#include <conio.h>
#include <consoleapi.h>

using namespace DX2DClasses;

void CDebugHelper::OpenConsole()
{
	AllocConsole(); //콘솔화면 만들기
}

void CDebugHelper::CloseConsole()
{
	FreeConsole(); //콘솔화면 끄기
}

void CDebugHelper::LogConsole(const char* msg, ...)
{
	va_list arglist;
	__crt_va_start(arglist, msg);
	_vcprintf_l(msg, NULL, arglist);
	__crt_va_end(arglist);
}
 
void CDebugHelper::DrawLine(SVector2& start, SVector2& end, CColorBrush* pColorBrush, float stroke)
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();
	pRenderTarget->DrawLine(start.ToPoint(), end.ToPoint(), pColorBrush->GetColorBrush(), stroke);
}

void CDebugHelper::DrawCircle(SVector2& pos, float rad, CColorBrush* pColorBrush, float stroke)
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();
	pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(pos.x, pos.y), rad, rad), pColorBrush->GetColorBrush(), stroke);
}
void CDebugHelper::DrawRect(SVector2& tl, SVector2& br, CColorBrush* pColorBrush, float stroke)
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();
	pRenderTarget->DrawRectangle(D2D1::RectF(tl.x, tl.y, br.x, br.y), pColorBrush->GetColorBrush(), stroke);
}

void CDebugHelper::DrawPolygon(SVector2 posArr[], int size, CColorBrush* pColorBrush, float stroke)
{
	for (int i = 0; i < size; i++)
	{
		SVector2 vStart = posArr[i];
		SVector2 vEnd;
		if (i == size - 1)
			vEnd = posArr[0];
		else
			vEnd = posArr[i + 1];
		//if(i == size-1) 
		//	vEnd = posArr[0];
		D2D1_POINT_2F posStart = D2D1::Point2F(vStart.x, vStart.y);
		D2D1_POINT_2F posEnd = D2D1::Point2F(vEnd.x, vEnd.y);
		ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();
		pRenderTarget->DrawLine(posStart, posEnd, pColorBrush->GetColorBrush());
	}
}

void CDebugHelper::DrawLine(D2D1::Matrix3x2F& mat, SVector2& start, SVector2& end, CColorBrush* pColorBrush, float stroke)
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();
	pRenderTarget->SetTransform(mat);
	pRenderTarget->DrawLine(start.ToPoint(), end.ToPoint(), pColorBrush->GetColorBrush(), stroke);
}

void CDebugHelper::DrawCircle(D2D1::Matrix3x2F& mat, SVector2& pos, float rad, CColorBrush* pColorBrush, float stroke)
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();
	pRenderTarget->SetTransform(mat);
	pRenderTarget->DrawEllipse(D2D1::Ellipse(D2D1::Point2F(pos.x, pos.y), rad, rad), pColorBrush->GetColorBrush(), stroke);
}
void CDebugHelper::DrawRect(D2D1::Matrix3x2F& mat, SVector2& tl, SVector2& br, CColorBrush* pColorBrush, float stroke)
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();
	pRenderTarget->SetTransform(mat);
	pRenderTarget->DrawRectangle(D2D1::RectF(tl.x, tl.y, br.x, br.y), pColorBrush->GetColorBrush(), stroke);
}

void CDebugHelper::DrawPolygon(D2D1::Matrix3x2F& mat, SVector2 posArr[],  int size, CColorBrush* pColorBrush, float stroke)
{
	for (int i = 0; i < size; i++)
	{
		SVector2 vStart = posArr[i];
		SVector2 vEnd;
		if(i == size-1) 
			vEnd = posArr[0];
		else
			vEnd = posArr[i + 1];
		//if(i == size-1) 
		//	vEnd = posArr[0];
		D2D1_POINT_2F posStart = D2D1::Point2F(vStart.x, vStart.y);
		D2D1_POINT_2F posEnd = D2D1::Point2F(vEnd.x, vEnd.y);
		ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();
		pRenderTarget->SetTransform(mat);
		pRenderTarget->DrawLine(posStart, posEnd, pColorBrush->GetColorBrush());
	}
}

void CDebugHelper::DrawCircle(CGameObject* pGameObject, CColorBrush* pColorBrush, float stroke)
{
	SVector2 vCenter = pGameObject->GetImage()->GetImageCenter();
	SVector2 vSize = pGameObject->GetImage()->GetImageSize() * 0.5f;
	float fRad = vSize.Magnitude();
	DrawCircle(vCenter, fRad, pColorBrush, stroke);
}

void CDebugHelper::DrawRect(CGameObject* pGameObject, CColorBrush* pColorBrush, float stroke)
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();
	SVector2 vSize = pGameObject->GetImage()->GetImageSize();
	pRenderTarget->SetTransform(pGameObject->GetTransform().GetTransfrom());
	pRenderTarget->DrawRectangle(D2D1::RectF(0, 0, vSize.x, vSize.y), pColorBrush->GetColorBrush(), stroke);
}

void CDebugHelper::DrawBox(CGameObject* pGameObject, CColorBrush* pColorBrush, float stroke)
{
	SVector2 posArr[4];
	SVector2 vSize = pGameObject->GetImage()->GetImageSize();

	posArr[0] = SVector2(0, 0);
	posArr[1] = SVector2(vSize.x, 0);
	posArr[2] = SVector2(vSize.x, vSize.y);
	posArr[3] = SVector2(0, vSize.y);

	DrawPolygon(pGameObject->GetTransform().GetTransfrom(), posArr, 4, pColorBrush, stroke);
}