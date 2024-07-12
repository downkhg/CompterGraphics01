#pragma once

#define ANI_SIZE 7

struct IWICFormatConverter;
struct ID2D1Bitmap;

class CImage
{
	IWICFormatConverter*		m_ipConvertedSrcBmp[ANI_SIZE]; //포맷변환기
	ID2D1Bitmap*				m_ipD2DBitmap[ANI_SIZE]; //비트맵
	D2D1_POINT_2F				m_sPointSize;

public:
	ID2D1HwndRenderTarget*		m_ipRT;
	IWICImagingFactory*			m_ipWICFactory ;

	D2D1_POINT_2F GetSize();
	D2D1_POINT_2F GetCenter();

	void CreateD2DBitmapFromFile(HWND hWnd, TCHAR* pImageFullPath, int idx);

	void ManualLoadImage(HWND hWnd, const TCHAR* format);

	void DrawBitmap(const D2D1_POINT_2F& pos, const D2D1_POINT_2F& size, const float& angle, int idx);
	void DrawBitmap(const D2D1::Matrix3x2F &mat, int idx);
};