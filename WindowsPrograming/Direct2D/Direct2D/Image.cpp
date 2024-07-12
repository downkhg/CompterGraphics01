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
	//	���ڴ��� �����Ѵ�.
	//----------------------------------------------------------------
	IWICBitmapDecoder* ipDecoderPtr = nullptr;
	hr = m_ipWICFactory->CreateDecoderFromFilename(pImageFullPath, nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&ipDecoderPtr);
	assert(hr == S_OK);

	//----------------------------------------------------------------
	//	���ڴ����� �������� ��´�.
	//----------------------------------------------------------------
	IWICBitmapFrameDecode* ipFramePtr = nullptr;
	hr = ipDecoderPtr->GetFrame(0, &ipFramePtr);
	assert(hr == S_OK);


	//----------------------------------------------------------------
	//	�������� ������� �ؼ� ���� �����͸� �����.
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
	//	����Ʈ�� �����͸� ������� �ؼ� ���� ��Ʈ���� �����.
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

	//D2D1::Matrix3x2F matTrans = matT * matR * matS; //2D������ �׻� 3��Ī���� �ٶ󺸱⶧���� ȸ���� �����ϸ� ������ ���������.
	D2D1::Matrix3x2F matTS = matT * matS;
	D2D1::Matrix3x2F matTrans = matT * matR * matS; //ȸ���� ������Ű�� �̵���ų ���, 2D���� �ɸ����� ��縸 ȸ����Ű�� ������ ó���� �����ϴ�. 

	D2D1_POINT_2F sTL = D2D1::Point2F(0, 0);
	D2D1_POINT_2F sTR = D2D1::Point2F(m_sPointSize.x, 0);
	D2D1_POINT_2F sBL = D2D1::Point2F(0, m_sPointSize.y);
	D2D1_POINT_2F sBR = m_sPointSize;

	D2D1_POINT_2F sRectTL = sTL * matT;
	D2D1_POINT_2F sRectBR = sBR * matT;

	D2D1_RECT_F sArea = D2D1::RectF(sRectTL.x, sRectTL.y, sRectBR.x, sRectBR.y);

	m_ipRT->SetTransform(matR * matT * matS);
	m_ipRT->DrawBitmap(bitmap, sArea);//��Ʈ���� �׸� ������ ȸ���� ������ �������� ��������.
}

void CImage::DrawBitmap(const D2D1::Matrix3x2F &mat, int idx)
{
	const D2D1_POINT_2F size = m_sPointSize;
	ID2D1Bitmap* bitmap = m_ipD2DBitmap[idx];

	D2D1_RECT_F sArea = D2D1::RectF(0, 0, m_sPointSize.x, m_sPointSize.y);
	m_ipRT->SetTransform(mat);//Ʈ�������� �����Ͽ� ��ġ�� �̵��Ѵ�.
	m_ipRT->DrawBitmap(bitmap, sArea);//��Ʈ���� �׸� ������ ȸ���� ������ �������� ��������.
}
