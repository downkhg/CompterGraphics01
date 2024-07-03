#include "Driect2DFramework.h"
#include "SingletonRenderTarget.h"
#include "DebugHelper.h"

#include <wincodec.h>
#include <d2d1.h>
#include <d2d1helper.h>

#include <assert.h>

using namespace DX2DClasses;

void CDriect2DFramwork::Initialize(HWND hWnd, int width, int height)
{
	_InitializeD2D(m_pD2DFactory, m_pWICImagingFactory);
	_InitializeRect(hWnd, m_pRenderTarget, width, height);
	CSingletonRenderTarget::SetRenderTarget(m_pRenderTarget);
}

void CDriect2DFramwork::Release()
{
	m_pRenderTarget->Release();
	m_pD2DFactory->Release();
	m_pWICImagingFactory->Release();
}

void CDriect2DFramwork::_InitializeD2D(ID2D1Factory* &pD2DFactory, IWICImagingFactory* &pWICImagingFactory)
{
	HRESULT hr = E_FAIL;

	//-----------------------------------------------------------------------
	//	D2D Factory 를 생성한다.
	//-----------------------------------------------------------------------
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
	assert(hr == S_OK);

	//-----------------------------------------------------------------------
	//	Windows Imaging Component Factory 를 생성한다.
	//-----------------------------------------------------------------------
	hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pWICImagingFactory));

	assert(hr == S_OK);
}

void CDriect2DFramwork::_InitializeRect(HWND hWnd, ID2D1HwndRenderTarget* &pRenderTaget, int width, int height)
{
	assert(m_pRenderTarget == nullptr);
	assert(hWnd != 0);

	HRESULT hr = E_FAIL;
	RECT rc;
	GetClientRect(hWnd, &rc);
	CDebugHelper::LogConsole("RectSize(%d,%d,%d,%d)", rc.left, rc.top, rc.right, rc.bottom);
	D2D1_RENDER_TARGET_PROPERTIES dxRTProperties = D2D1::RenderTargetProperties();
	dxRTProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);
	dxRTProperties.dpiX = 72.0f;
	dxRTProperties.dpiY = 72.0f;

	D2D1_SIZE_U dxSize;
	if(width == 0 || height == 0)
		dxSize = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
	else
		dxSize = D2D1::SizeU(width,height);

	hr = m_pD2DFactory->CreateHwndRenderTarget(dxRTProperties, D2D1::HwndRenderTargetProperties(hWnd, dxSize), &pRenderTaget);

	assert(hr == S_OK);
}
