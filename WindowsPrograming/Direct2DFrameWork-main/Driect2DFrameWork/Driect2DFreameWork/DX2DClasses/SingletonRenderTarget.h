#pragma once

#define NULL 0

struct ID2D1HwndRenderTarget;

namespace DX2DClasses
{
	class CSingletonRenderTarget
	{
	protected:
		static ID2D1HwndRenderTarget* m_pRenderTarget;
	public:
		CSingletonRenderTarget(ID2D1HwndRenderTarget* pRenderTarget = NULL);
		static void CreateRenderTarget();
		static ID2D1HwndRenderTarget* GetRenderTarget();
		static void SetRenderTarget(ID2D1HwndRenderTarget* pRenderTarget);
	};
}