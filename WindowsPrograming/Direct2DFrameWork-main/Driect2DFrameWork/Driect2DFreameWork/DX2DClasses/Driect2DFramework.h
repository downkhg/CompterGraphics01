#pragma once
#include <Windows.h>

struct ID2D1Factory;
struct ID2D1HwndRenderTarget;
struct IWICImagingFactory;

namespace DX2DClasses
{
	class CDriect2DFramwork
	{
		ID2D1Factory* m_pD2DFactory = nullptr;
		ID2D1HwndRenderTarget* m_pRenderTarget = nullptr;
		IWICImagingFactory* m_pWICImagingFactory = nullptr;
	public:
		ID2D1Factory* GetD2DFactory() { return m_pD2DFactory; }
		ID2D1HwndRenderTarget* GetD2DRenderTarget() { return m_pRenderTarget; }
		IWICImagingFactory* GetImagingFactory(){ return m_pWICImagingFactory; }

		void Initialize(HWND hWnd, int width = 0, int height = 0);
		void Release();
	private:
		void _InitializeD2D(ID2D1Factory* &pD2DFactory, IWICImagingFactory* &pWICImagingFactory);
		void _InitializeRect(HWND hWnd, ID2D1HwndRenderTarget*& pRenderTaget, int width = 0, int height = 0);
	};
}