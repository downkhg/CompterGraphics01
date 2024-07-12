#include "stdafx.h"
#include "Image.h"
#include <cassert>
#include <commdlg.h>

#include <wincodec.h>

#include <d2d1.h>
#include <d2d1helper.h>


D2D1_POINT_2F CImage::GetSize() { return m_sPointSize; }
D2D1_POINT_2F CImage::GetCenter() { return D2D1::Point2F(m_sPointSize.x / 2, m_sPointSize.y / 2); }

void CImage::CreateD2DBitmapFromFile(HWND hWnd, TCHAR* pImageFullPath, int idx)
{
	IWICFormatConverter* &ipConvertedSrcBmp = m_ipConvertedSrcBmp[idx];
	ID2D1Bitmap* &ipD2DBitmap = m_ipD2DBitmap[idx];

	assert(pImageFullPath != nullptr);
	//assert(g_ipWICFactory != nullptr);
	//assert(g_ipRT != nullptr);

	HRESULT hr = E_FAIL;

	//----------------------------------------------------------------
	//	디코더를 생성한다.
	//----------------------------------------------------------------
	IWICBitmapDecoder* ipDecoderPtr = nullptr;
	hr = m_ipWICFactory->CreateDecoderFromFilename(pImageFullPath, nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&ipDecoderPtr);
	assert(hr == S_OK);

	//----------------------------------------------------------------
	//	디코더에서 프레임을 얻는다.
	//----------------------------------------------------------------
	IWICBitmapFrameDecode* ipFramePtr = nullptr;
	hr = ipDecoderPtr->GetFrame(0, &ipFramePtr);
	assert(hr == S_OK);


	//----------------------------------------------------------------
	//	프레임을 기반으로 해서 포맷 컨버터를 만든다.
	//----------------------------------------------------------------
	SafeRelease(ipConvertedSrcBmp);
	hr = m_ipWICFactory->CreateFormatConverter(&ipConvertedSrcBmp);
	assert(hr == S_OK);


	hr = ipConvertedSrcBmp->Initialize(ipFramePtr, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom);
	assert(hr == S_OK);

	//----------------------------------------------------------------
	//	컨버트된 데이터를 기반으로 해서 실제 비트맵을 만든다.
	//----------------------------------------------------------------
	SafeRelease(ipD2DBitmap);
	hr = m_ipRT->CreateBitmapFromWicBitmap(ipConvertedSrcBmp, nullptr, &ipD2DBitmap);
	assert(hr == S_OK);

	SafeRelease(ipDecoderPtr);
	SafeRelease(ipFramePtr);
}

void CImage::ManualLoadImage(HWND hWnd, const TCHAR* format)
{
	TCHAR szFullPath[1024];

	for (int i = 0; i < ANI_SIZE; i++)
	{
		wsprintf(szFullPath, format, i);
		CreateD2DBitmapFromFile(hWnd, szFullPath, i);

		RECT rc;
		m_sPointSize = D2D1::Point2F(m_ipD2DBitmap[i]->GetSize().width, m_ipD2DBitmap[i]->GetSize().height);
		SetRect(&rc, 100, 100, m_sPointSize.x, m_sPointSize.y);
		InvalidateRect(hWnd, &rc, TRUE);
	}
}

void CImage::DrawBitmap(const D2D1_POINT_2F& pos, const D2D1_POINT_2F& size, const float& angle, int idx)
{
	ID2D1Bitmap* bitmap = m_ipD2DBitmap[idx];

	D2D1::Matrix3x2F matT = D2D1::Matrix3x2F::Translation(pos.x, pos.y);
	//D2D1::Matrix3x2F matR = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F matR = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(m_sPointSize.x / 2, m_sPointSize.y / 2));
	D2D1::Matrix3x2F matS = D2D1::Matrix3x2F::Scale(size.x, size.y);

	//D2D1::Matrix3x2F matTrans = matT * matR * matS; //2D에서는 항상 3인칭으로 바라보기때문에 회전을 먼저하면 조작이 어려워진다.
	D2D1::Matrix3x2F matTS = matT * matS;
	D2D1::Matrix3x2F matTrans = matT * matR * matS; //회전을 먼저시키고 이동시킬 경우, 2D에서 케릭터의 모양만 회전시키는 식으로 처리가 가능하다. 

	D2D1_POINT_2F sTL = D2D1::Point2F(0, 0);
	D2D1_POINT_2F sTR = D2D1::Point2F(m_sPointSize.x, 0);
	D2D1_POINT_2F sBL = D2D1::Point2F(0, m_sPointSize.y);
	D2D1_POINT_2F sBR = m_sPointSize;

	D2D1_POINT_2F sRectTL = sTL * matT;
	D2D1_POINT_2F sRectBR = sBR * matT;

	D2D1_RECT_F sArea = D2D1::RectF(sRectTL.x, sRectTL.y, sRectBR.x, sRectBR.y);

	m_ipRT->SetTransform(matR * matT * matS);
	m_ipRT->DrawBitmap(bitmap, sArea);//비트맵을 그릴 영역은 회전을 적용한 랜더링이 되지않음.
}

void CImage::DrawBitmap(const D2D1::Matrix3x2F &mat, int idx)
{
	const D2D1_POINT_2F size = m_sPointSize;
	ID2D1Bitmap* bitmap = m_ipD2DBitmap[idx];

	D2D1_RECT_F sArea = D2D1::RectF(0, 0, m_sPointSize.x, m_sPointSize.y);
	m_ipRT->SetTransform(mat);//트랜스폼을 셋팅하여 위치를 이동한다.
	m_ipRT->DrawBitmap(bitmap, sArea);//비트맵을 그릴 영역은 회전을 적용한 랜더링이 되지않음.
}
