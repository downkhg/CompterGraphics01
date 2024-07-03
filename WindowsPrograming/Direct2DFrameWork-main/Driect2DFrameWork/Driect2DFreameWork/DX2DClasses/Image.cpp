#include "Image.h"
#include "ColorBrush.h"
#include <cassert>
#include <commdlg.h>

#include <wincodec.h>

#include <d2d1.h>
#include <d2d1helper.h>

using namespace DX2DClasses;

SVector2 CImage::GetImageSize() { return m_sPointSize; }
SVector2 CImage::GetImageCenter() { return SVector2(m_sPointSize.x / 2, m_sPointSize.y / 2); }
int CImage::GetAnimationCount() { return m_nAnimSize; }

CImage::CImage(ID2D1HwndRenderTarget* pRenderTarget, IWICImagingFactory* pWICFactory, int nSize)
{
	m_pRenderTarget = pRenderTarget;
	m_pWICFactory = pWICFactory;
	//m_pD2DBitmap = new ID2D1Bitmap*[nSize];
	m_pD2DBitmap.resize(nSize);
	m_nAnimSize = nSize;
}
CImage::~CImage()
{
	/*if (m_pD2DBitmap)
	{
		delete[] m_pD2DBitmap;
		m_pD2DBitmap = NULL;
	}*/
	m_pD2DBitmap.clear();
}

void CImage::_CreateD2DBitmapFromFile(HWND hWnd, TCHAR* pImageFullPath, int idx)
{
	IWICFormatConverter* &ipConvertedSrcBmp = m_pConvertedSrcBmp;
	ID2D1Bitmap* &ipD2DBitmap = m_pD2DBitmap[idx];

	assert(pImageFullPath != nullptr);
	//assert(g_ipWICFactory != nullptr);
	//assert(g_ipRT != nullptr);

	HRESULT hr = E_FAIL;

	//----------------------------------------------------------------
	//	���ڴ��� �����Ѵ�.
	//----------------------------------------------------------------
	IWICBitmapDecoder* ipDecoderPtr = nullptr;
	hr = m_pWICFactory->CreateDecoderFromFilename(pImageFullPath, nullptr,
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
	hr = m_pWICFactory->CreateFormatConverter(&ipConvertedSrcBmp);
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
	hr = m_pRenderTarget->CreateBitmapFromWicBitmap(ipConvertedSrcBmp, nullptr, &ipD2DBitmap);
	assert(hr == S_OK);

	ipDecoderPtr->Release();
	ipFramePtr->Release();
}

void CImage::ManualLoadImage(HWND hWnd, const TCHAR* format)
{
	TCHAR szFullPath[1024];

	for (int i = 0; i < m_nAnimSize; i++)
	{
		wsprintf(szFullPath, format, i);
		_CreateD2DBitmapFromFile(hWnd, szFullPath, i);

		RECT rc;
		m_sPointSize = SVector2(m_pD2DBitmap[i]->GetSize().width, m_pD2DBitmap[i]->GetSize().height);
		SetRect(&rc, 100, 100, m_sPointSize.x, m_sPointSize.y);
		InvalidateRect(hWnd, &rc, TRUE);
	}
}

void CImage::DrawBitmap(const SVector2& pos, const SVector2& size, const float& angle, int idx)
{
	ID2D1Bitmap* bitmap = m_pD2DBitmap[idx];

	D2D1::Matrix3x2F matT = D2D1::Matrix3x2F::Translation(pos.x, pos.y);
	//D2D1::Matrix3x2F matR = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F matR = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(m_sPointSize.x / 2, m_sPointSize.y / 2));
	D2D1::Matrix3x2F matS = D2D1::Matrix3x2F::Scale(size.x, size.y);

	//D2D1::Matrix3x2F matTrans = matT * matR * matS; //2D������ �׻� 3��Ī���� �ٶ󺸱⶧���� ȸ���� �����ϸ� ������ ���������.
	D2D1::Matrix3x2F matTS = matT * matS;
	D2D1::Matrix3x2F matTrans = matT * matR * matS; //ȸ���� ������Ű�� �̵���ų ���, 2D���� �ɸ����� ��縸 ȸ����Ű�� ������ ó���� �����ϴ�. 

	D2D1_RECT_F sArea = D2D1::RectF(0, 0, m_sPointSize.x, m_sPointSize.y);

	m_pRenderTarget->SetTransform(matTrans);
	m_pRenderTarget->DrawBitmap(bitmap, sArea);//��Ʈ���� �׸� ������ ȸ���� ������ �������� ��������.

}

void CImage::DrawBitmap(const D2D1::Matrix3x2F &mat, int idx)
{
	const SVector2 size = m_sPointSize;
	ID2D1Bitmap* bitmap = m_pD2DBitmap[idx];

	D2D1_RECT_F sArea = D2D1::RectF(0, 0, m_sPointSize.x, m_sPointSize.y);
	m_pRenderTarget->SetTransform(mat);//Ʈ�������� �����Ͽ� ��ġ�� �̵��Ѵ�.
	m_pRenderTarget->DrawBitmap(bitmap, sArea);//��Ʈ���� �׸� ������ ȸ���� ������ �������� ��������.
}

void CImage::DrawBitmap(const D2D1::Matrix3x2F* mat, int idx)
{
	const SVector2 size = m_sPointSize;
	ID2D1Bitmap* bitmap = m_pD2DBitmap[idx];

	D2D1_RECT_F sArea = D2D1::RectF(0, 0, m_sPointSize.x, m_sPointSize.y);
	m_pRenderTarget->SetTransform(mat);//Ʈ�������� �����Ͽ� ��ġ�� �̵��Ѵ�.
	m_pRenderTarget->DrawBitmap(bitmap, sArea);//��Ʈ���� �׸� ������ ȸ���� ������ �������� ��������.
}
