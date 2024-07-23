// DIB module

#include <windows.h>
#include <stdio.h>
#include <math.h>

#include "dib.h"

#define DEFAULT_XPIXELSPERMETER	3000
#define DEFAULT_YPIXELSPERMETER	3000


////////////////////////////////////////////////////////////////////////////////
//	file io

LPBYTE	DibLoadHandle( const char* lpFileName )
{
	FILE				*fp;
	BITMAPFILEHEADER	bmfh;
	HGLOBAL				hDib;
	LPBYTE				lpDib;
	int					nSize;

	fopen_s( &fp, lpFileName, "rb" );
	if ( !fp ) return NULL;

	fread( &bmfh, sizeof(BITMAPFILEHEADER), 1, fp );
	nSize = bmfh.bfSize - sizeof(BITMAPFILEHEADER);
	hDib = (HGLOBAL) GlobalAlloc( GMEM_FIXED, nSize );
	lpDib = (LPBYTE) GlobalLock( hDib );
	if ( !lpDib )
	{
		fclose( fp );
		return NULL;
	}
	fread( lpDib, nSize, 1, fp );
	fclose( fp );

	return lpDib;
}

bool	DibSave( LPBYTE lpDib, const char* lpFileName )
{
	FILE				*fp;
	BITMAPFILEHEADER	bmfh;

	if ( !lpDib || !lpFileName ) return false;
	fopen_s( &fp, lpFileName, "wb" );
	if ( !fp ) return false;
	
	bmfh.bfType = 0x4d42;
	bmfh.bfSize = sizeof(BITMAPFILEHEADER) + DIB_SIZE( lpDib );
	bmfh.bfReserved1 = 0;
	bmfh.bfReserved2 = 0;
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + DIB_PALSIZE( lpDib );
	fwrite( &bmfh, sizeof(BITMAPFILEHEADER), 1, fp );
	fwrite( lpDib, DIB_SIZE( lpDib ), 1, fp );
	fclose( fp );

	return true;
}


////////////////////////////////////////////////////////////////////////////////
//	creation

LPBYTE	DibCreateEmpty( int nBitsPerPixel, int nWidth, int nHeight )
{
	LPBITMAPINFOHEADER	lpBMIH;
	LPBYTE	lpDib;
	int		nPal = nBitsPerPixel == 24 ? 0 : 1 << nBitsPerPixel;
	int		nPalSize = sizeof(RGBQUAD) * nPal;
	int		nLineSize = ALIGN_4B(nWidth * nBitsPerPixel / 8);
	DWORD	dwSize = sizeof(BITMAPINFOHEADER) + nPalSize + nLineSize * nHeight;

	lpDib = new BYTE [dwSize];
	memset( lpDib, 0xff, dwSize );
	lpBMIH = DIB_HEADER(lpDib);
	lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
	lpBMIH->biWidth = nWidth;
	lpBMIH->biHeight = nHeight;
	lpBMIH->biPlanes = 1;
	lpBMIH->biBitCount = nBitsPerPixel;
	lpBMIH->biCompression = BI_RGB;
	lpBMIH->biSizeImage = 0;
	lpBMIH->biXPelsPerMeter = DEFAULT_XPIXELSPERMETER;
	lpBMIH->biYPelsPerMeter = DEFAULT_YPIXELSPERMETER;
	lpBMIH->biClrUsed = nPal;
	lpBMIH->biClrImportant = nPal;

	RGBQUAD	rgbQuad;
	int		i;
	for ( i = 0; i < nPal; i++ )
	{
		rgbQuad.rgbBlue = i;
		rgbQuad.rgbGreen = i;
		rgbQuad.rgbRed = i;
		rgbQuad.rgbReserved = 0;
		DIB_BMI( lpDib )->bmiColors[i] = rgbQuad;
	}

	return lpDib;
}

// WIN 16 버젼 라이브러리들과의 호환으로 준비.
LPBYTE DibCreateEmptyHandle( int nBitsPerPixel, int nWidth, int nHeight )
{
	LPBITMAPINFOHEADER	lpBMIH;
	HGLOBAL hDib;
	LPBYTE	lpDib;
	int		nPal = nBitsPerPixel == 24 ? 0 : 1 << nBitsPerPixel;
	int		nPalSize = sizeof(RGBQUAD) * nPal;
	int		nLineSize = ALIGN_4B(nWidth * nBitsPerPixel / 8);
	DWORD	dwSize = sizeof(BITMAPINFOHEADER) + nPalSize + nLineSize * nHeight;

	hDib = (HGLOBAL) GlobalAlloc( GMEM_FIXED, dwSize );
	lpDib = (LPBYTE) GlobalLock( hDib );
	memset( lpDib, 0xff, dwSize );
	lpBMIH = (LPBITMAPINFOHEADER) lpDib;
	lpBMIH->biSize = sizeof(BITMAPINFOHEADER);
	lpBMIH->biWidth = nWidth;
	lpBMIH->biHeight = nHeight;
	lpBMIH->biPlanes = 1;
	lpBMIH->biBitCount = nBitsPerPixel;
	lpBMIH->biCompression = BI_RGB;
	lpBMIH->biSizeImage = 0;
	lpBMIH->biXPelsPerMeter = DEFAULT_XPIXELSPERMETER;
	lpBMIH->biYPelsPerMeter = DEFAULT_YPIXELSPERMETER;
	lpBMIH->biClrUsed = nPal;
	lpBMIH->biClrImportant = nPal;

	RGBQUAD	rgbQuad;
	int		i;
	for ( i = 0; i < nPal; i++ )
	{
		rgbQuad.rgbBlue = i;
		rgbQuad.rgbGreen = i;
		rgbQuad.rgbRed = i;
		rgbQuad.rgbReserved = 0;
		DIB_BMI( lpDib )->bmiColors[i] = rgbQuad;
	}

	return lpDib;
}

void	DibDeleteHandle( LPBYTE lpSrc )
{
	if ( lpSrc )
	{
		GlobalUnlock( lpSrc );
		GlobalFree( lpSrc );
	}
}


////////////////////////////////////////////////////////////////////////////////
//	copy

LPBYTE	DibDuplicate( LPBYTE lpSrc )
{
	if ( !lpSrc )
		return NULL;

	LPBYTE	lpDib;
	DWORD	dwSize = DIB_SIZE( lpSrc );

	lpDib = new BYTE [dwSize];
	if ( lpDib )
		memcpy( lpDib, lpSrc, dwSize );

	return lpDib;
}

LPBYTE	DibDuplicateHandle( LPBYTE lpSrc )
{
	if ( !lpSrc )
		return NULL;

	HGLOBAL hDib;
	LPBYTE	lpDib;
	DWORD	dwSize = DIB_SIZE( lpSrc );

	hDib = (HGLOBAL) GlobalAlloc( GMEM_FIXED, dwSize );
	lpDib = (LPBYTE) GlobalLock( hDib );
	if ( lpDib )
		memcpy( lpDib, lpSrc, dwSize );

	return lpDib;
}

bool	DibCopyRect( LPBYTE lpDstDib, int dstx, int dsty, LPBYTE lpSrcDib, LPRECT prSrc1 )
{
	if ( !lpDstDib || !lpSrcDib || DIB_BPP( lpDstDib ) != DIB_BPP( lpSrcDib ) )
		return false;

	RECT	rDst, rDst1, rDst2, rSrc2, rSrc;

	SetRect( &rSrc2, 0, 0, DIB_CX( lpSrcDib ), DIB_CY( lpSrcDib ) );
	IntersectRect( &rSrc, prSrc1, &rSrc2 );
	SetRect( &rDst1, 0, 0, rSrc.right - rSrc.left, rSrc.bottom - rSrc.top );
	OffsetRect( &rDst1, dstx, dsty );
	SetRect( &rDst2, 0, 0, DIB_CX( lpDstDib ), DIB_CY( lpDstDib ) );
	IntersectRect( &rDst, &rDst1, &rDst2 );

	LPBYTE	lpSrc, lpDst;
	int		y, nWidth = (rDst.right - rDst.left) * DIB_BPP( lpSrcDib ) / 8;

	for ( y = rDst.top; y < rDst.bottom; y++ )
	{
		lpSrc = DIB_DATAXY_INV( lpSrcDib, rSrc.left, rSrc.top + y - rDst.top );
		lpDst = DIB_DATAXY_INV( lpDstDib, rDst.left, y );
		memcpy( lpDst, lpSrc, nWidth );
	}

	return true;
}

bool	DibCopyRectROP( LPBYTE lpDstDib, int dstx, int dsty, LPBYTE lpSrcDib, LPRECT prSrc1, DWORD dwROP )
{
	if ( !lpDstDib || !lpSrcDib || DIB_BPP( lpDstDib ) != DIB_BPP( lpSrcDib ) )
		return false;

	RECT	rDst, rDst1, rDst2, rSrc2, rSrc;

	SetRect( &rSrc2, 0, 0, DIB_CX( lpSrcDib ), DIB_CY( lpSrcDib ) );
	IntersectRect( &rSrc, prSrc1, &rSrc2 );
	SetRect( &rDst1, 0, 0, rSrc.right - rSrc.left, rSrc.bottom - rSrc.top );
	OffsetRect( &rDst1, dstx, dsty );
	SetRect( &rDst2, 0, 0, DIB_CX( lpDstDib ), DIB_CY( lpDstDib ) );
	IntersectRect( &rDst, &rDst1, &rDst2 );

	LPBYTE	lpSrc, lpDst;
	int		y, i, nWidth = (rDst.right - rDst.left) * DIB_BPP( lpSrcDib ) / 8;

	for ( y = rDst.top; y < rDst.bottom; y++ )
	{
		lpSrc = DIB_DATAXY_INV( lpSrcDib, rSrc.left, rSrc.top + y - rDst.top );
		lpDst = DIB_DATAXY_INV( lpDstDib, rDst.left, y );
		switch ( dwROP )
		{
		case SRCAND:
			for ( i = 0; i < nWidth; i++ )
			{
				*lpDst = *lpDst & *lpSrc;
				lpDst++;
				lpSrc++;
			}
			break;
		case SRCPAINT:
			for ( i = 0; i < nWidth; i++ )
			{
				*lpDst = *lpDst | *lpSrc;
				lpDst++;
				lpSrc++;
			}
			break;
		case SRCCOPY:
			memcpy( lpDst, lpSrc, nWidth );
			break;
		default:
			break;
		}
	}

	return true;
}

bool	DibCopy( LPBYTE lpDst, LPBYTE lpSrc )
{
	if ( !lpSrc || !lpDst || DIB_BPP( lpSrc ) != DIB_BPP( lpDst ) ) return false;
	if ( DIB_CX( lpSrc ) != DIB_CX( lpDst ) || DIB_CY( lpSrc ) != DIB_CY( lpDst ) ) return false;

	memcpy( DIB_DATA( lpDst ), DIB_DATA( lpSrc ), DIB_DATASIZE( lpSrc ) );

	return true;
}

bool	DibTile( LPBYTE lpSrcDib, LPBYTE lpTileDib )
{
	LPBYTE	lpSrc, lpTile;
	int		y, cx, cy, tcx, tcy, n, i, bytesps;

	if ( !lpSrcDib || !lpTileDib || DIB_BPP( lpSrcDib ) != DIB_BPP( lpTileDib ) )
		return false;

	bytesps = DIB_BPP( lpSrcDib ) / 8;
	cx = DIB_CX( lpSrcDib );
	cy = DIB_CY( lpSrcDib );
	tcx = DIB_CX( lpTileDib );
	tcy = DIB_CY( lpTileDib );

	for ( y = 0; y < cy; y++ )
	{
		lpTile = DIB_DATAXY_INV( lpTileDib, 0, y % tcy );
		if ( cx > tcx )
		{
			n = cx / tcx + (cx % tcx ? 1 : 0);
			for ( i = 0; i < n; i++ )
			{
				lpSrc = DIB_DATAXY_INV( lpSrcDib, i * tcx, y );
				memcpy( lpSrc, lpTile, min(cx - (i * tcx), tcx) * bytesps );
			}
		}
		else
		{
			lpSrc = DIB_DATAXY_INV( lpSrcDib, 0, y );
			memcpy( lpSrc, lpTile, cx * bytesps );
		}
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////////
//	geometry

LPBYTE	DibFlipHandle( LPBYTE lpDib )
{
	if ( !lpDib )
		return NULL;

	LPBYTE	lpNewDib;
	int		y;
	lpNewDib = (LPBYTE) GlobalLock( DibCreateEmptyHandle( DIB_BPP( lpDib ), DIB_CX( lpDib ), DIB_CY( lpDib ) ) );
	if ( lpNewDib )
	{
		for ( y = 0; y < DIB_CY( lpDib ); y++ )
		{
			memcpy( DIB_DATAXY( lpNewDib, 0, y ), DIB_DATAXY_INV( lpDib, 0, y ), DIB_LINESIZE( lpDib ) );
		}
	}

	return lpNewDib;
}

LPBYTE	DibReverseHandle( LPBYTE lpDib )
{
	if ( !lpDib )
		return NULL;

	LPBYTE	lpNewDib, lpNewData, lpData;
	int		x, y;
	lpNewDib = (LPBYTE) GlobalLock( DibCreateEmptyHandle( DIB_BPP( lpDib ), DIB_CX( lpDib ), DIB_CY( lpDib ) ) );
	if ( lpNewDib )
	{
		for ( y = 0; y < DIB_CY( lpDib ); y++ )
		{
			lpData = DIB_DATAXY( lpDib, 0, y );
			lpNewData = DIB_DATAXY( lpNewDib, DIB_CX( lpNewDib ) - 1, y );
			if ( DIB_BPP( lpDib ) == 8 )
			{
				for ( x = 0; x < DIB_CX( lpDib ); x++ )
					*lpNewData-- = *lpData++;
			}
			else if ( DIB_BPP( lpDib ) == 24 )
			{
				for ( x = 0; x < DIB_CX( lpDib ); x++ )
				{
					*lpNewData = *lpData++;
					*(lpNewData + 1) = *lpData++;
					*(lpNewData + 2) = *lpData++;
					lpNewData -= 3;
				}
			}
		}
	}

	return lpNewDib;
}

LPBYTE	DibRotateHandle( LPBYTE lpDib, float fDegree )
{
	if ( !lpDib )
		return NULL;

	LPBYTE	lpNewDib = NULL, lpNewData, lpData;
	int		x, y, cx, cy;
	cx = DIB_CX( lpDib );
	cy = DIB_CY( lpDib );

	if ( fDegree == 90.0f )
	{
		if ( !(lpNewDib = (LPBYTE) GlobalLock( DibCreateEmptyHandle( DIB_BPP( lpDib ), cy, cx ) )) )
			return NULL;

		for ( y = 0; y < cy; y++ )
		{
			lpData = DIB_DATAXY( lpDib, 0, y );
			lpNewData = DIB_DATAXY( lpNewDib, y, cx - 1 );
			if ( DIB_BPP( lpDib ) == 8 )
			{
				for ( x = 0; x < cx; x++, lpNewData -= ALIGN_4B( cy ) )
					*lpNewData = *lpData++;
			}
			else if ( DIB_BPP( lpDib ) == 24 )
			{
				for ( x = 0; x < cx; x++, lpNewData -= ALIGN_4B( cy * 3 ) )
				{
					*lpNewData = *lpData++;
					*(lpNewData + 1) = *lpData++;
					*(lpNewData + 2) = *lpData++;
				}
			}
		}
	}
	else if ( fDegree == 180.0f )
	{
		if ( !(lpNewDib = (LPBYTE) GlobalLock( DibCreateEmptyHandle( DIB_BPP( lpDib ), cx, cy ) )) )
			return NULL;

		for ( y = 0; y < cy; y++ )
		{
			lpData = DIB_DATAXY( lpDib, 0, y );
			lpNewData = DIB_DATAXY( lpNewDib, cx - 1, cy - 1 - y );
			if ( DIB_BPP( lpDib ) == 8 )
			{
				for ( x = 0; x < cx; x++, lpNewData-- )
					*lpNewData = *lpData++;
			}
			else if ( DIB_BPP( lpDib ) == 24 )
			{
				for ( x = 0; x < cx; x++, lpNewData -= 3 )
				{
					*lpNewData = *lpData++;
					*(lpNewData + 1) = *lpData++;
					*(lpNewData + 2) = *lpData++;
				}
			}
		}
	}
	else if ( fDegree == 270.0f )
	{
		if ( !(lpNewDib = (LPBYTE) GlobalLock( DibCreateEmptyHandle( DIB_BPP( lpDib ), cy, cx ) )) )
			return NULL;

		for ( y = 0; y < cy; y++ )
		{
			lpData = DIB_DATAXY( lpDib, 0, y );
			lpNewData = DIB_DATAXY( lpNewDib, cy - 1 - y, 0 );
			if ( DIB_BPP( lpDib ) == 8 )
			{
				for ( x = 0; x < cx; x++, lpNewData += ALIGN_4B( cy ) )
					*lpNewData = *lpData++;
			}
			else if ( DIB_BPP( lpDib ) == 24 )
			{
				for ( x = 0; x < cx; x++, lpNewData += ALIGN_4B( cy * 3 ) )
				{
					*lpNewData = *lpData++;
					*(lpNewData + 1) = *lpData++;
					*(lpNewData + 2) = *lpData++;
				}
			}
		}
	}
	else // free angle
	{
/*		CLeadBitmap	lBitmap;

		lBitmap.ConvertFromDIB( (LPBITMAPINFO)lpDib, lpDib );
		lBitmap.Rotate( nNewWidth, nNewHeight );
		lpNewDib = (LPBYTE) GlobalLock( lBitmap.ConvertToDIB() );
*/	}

	return lpNewDib;
}

LPBYTE	DibResizeHandle( LPBYTE lpDib, int nNewWidth, int nNewHeight )
{
	if ( !lpDib || nNewWidth <= 0 || nNewHeight <= 0 )
		return NULL;

	return NULL;
}

LPBYTE	DibReduce( LPBYTE lpSrcDib, int newCx, int newCy )
{
	LPBYTE	lpDstDib, lpSrc, lpDst, lpTemp;
	float	fXratio, fYratio;
	int		x, y, cx, cy, nBps;

	if ( !lpSrcDib || newCx <= 0 || newCy <= 0 )
		return NULL;

	cx = DIB_CX( lpSrcDib );
	cy = DIB_CY( lpSrcDib );
	nBps = DIB_BPP( lpSrcDib );
	if ( nBps != 24 && nBps != 8 )
		return NULL;

	fXratio = (float)cx / newCx;
	fYratio = (float)cy / newCy;

	if ( (lpDstDib = DibCreateEmpty( nBps, newCx, newCy )) == NULL )
		return NULL;

	if ( nBps == 8 )
	{
		for ( y = 0; y < newCy; y++ )
		{
			lpSrc = DIB_ALPHAXY( lpSrcDib, 0, ((int)(y * fYratio)) );
			lpDst = DIB_ALPHAXY( lpDstDib, 0, y );
			for ( x = 0; x < newCx; x++ )
				*lpDst++ = *(lpSrc + (int)(x * fXratio));
		}
	}
	else if ( nBps == 24 )
	{
		for ( y = 0; y < newCy; y++ )
		{
			lpSrc = DIB_IMAGEXY( lpSrcDib, 0, ((int)(y * fYratio)) );
			lpDst = DIB_IMAGEXY( lpDstDib, 0, y );
			for ( x = 0; x < newCx; x++ )
			{
				lpTemp = lpSrc + (int)(x * fXratio) * 3;
				*lpDst++ = *lpTemp++;
				*lpDst++ = *lpTemp++;
				*lpDst++ = *lpTemp++;
			}
		}
	}

	return lpDstDib;
}


////////////////////////////////////////////////////////////////////////////////
//	convert format

LPBYTE	DibRGB2GRAYHandle( LPBYTE lpRgbDib )
{
	LPBYTE	lpGrayDib, lpSrc, lpDst;
	int		x, y, cx, cy;

	if ( !lpRgbDib || DIB_BPP( lpRgbDib ) != 24 )
		return NULL;

	cx = DIB_CX( lpRgbDib );
	cy = DIB_CY( lpRgbDib );
	if ( !(lpGrayDib = DibCreateEmptyHandle( 8, cx, cy )) )
		return NULL;

	for ( y = 0; y < cy; y++ )
	{
		lpSrc = DIB_DATA24XY( lpRgbDib, 0, y );
		lpDst = DIB_DATA8XY( lpGrayDib, 0, y );
		for ( x = 0; x < cx; x++ )
		{
			*lpDst++ = (*lpSrc + *(lpSrc+1) + *(lpSrc+2))/3;
			lpSrc += 3;
		}
	}

	return lpGrayDib;
}

LPBYTE	DibGRAY2RGBHandle( LPBYTE lpGrayDib )
{
	LPBYTE	lpRgbDib, lpSrc, lpDst;
	int		x, y, cx, cy;

	if ( !lpGrayDib || DIB_BPP( lpGrayDib ) != 8 )
		return NULL;

	cx = DIB_CX( lpGrayDib );
	cy = DIB_CY( lpGrayDib );
	if ( !(lpRgbDib = DibCreateEmptyHandle( 24, cx, cy )) )
		return NULL;

	for ( y = 0; y < cy; y++ )
	{
		lpSrc = DIB_DATA8XY( lpGrayDib, 0, y );
		lpDst = DIB_DATA24XY( lpRgbDib, 0, y );
		for ( x = 0; x < cx; x++ )
		{
			*lpDst++ = *lpSrc;
			*lpDst++ = *lpSrc;
			*lpDst++ = *lpSrc;
			lpSrc ++;
		}
	}

	return lpRgbDib;
}


////////////////////////////////////////////////////////////////////////////////
//	etc

bool	DibPrint( HWND hwnd, LPBYTE lpDib, const char* lpszDevice, int nDevide )
{
/*
	BITMAPHANDLE	LBitmap;	// Bitmap handle for the image 
	HDC				PrinterDC;	// DC for the printer 
	int				nWidthAllowed, nHeightAllowed, nWidthFactor, nHeightFactor; // For size calculations 
	int				nWidthPrint, nHeightPrint; // nWidth and height of the printed image 
	DOCINFO			DocInfo;	// Document information structure 

	if ( !lpDib )
		return false;

	PrinterDC = CreateDC( NULL, lpszDevice, NULL, NULL );
	if ( !PrinterDC )
		return false;

	L_InitBitmap( &LBitmap, 0, 0, DIB_BPP( lpDib ) );
	L_ConvertFromDIB( &LBitmap, (LPBITMAPINFO)lpDib, DIB_DATA( lpDib ) );

	// Initialize the document to be printed 
	memset( &DocInfo, 0, sizeof(DocInfo) );
	DocInfo.cbSize = sizeof(DocInfo);
	DocInfo.lpszDocName = "Portrait or Caricature";
	// Initialize variables for fitting the image on half a page 
	nWidthAllowed = GetDeviceCaps( PrinterDC, HORZRES );
	nHeightAllowed = GetDeviceCaps( PrinterDC, VERTRES );
	nHeightFactor = BITMAPHEIGHT( &LBitmap );
	nWidthFactor = BITMAPWIDTH( &LBitmap );
	// See if using the maximum width will make the image too tall 
	if ( ((nWidthAllowed * nHeightFactor) / nWidthFactor) < nHeightAllowed )
	{	// Use the maximum width, and calculate the height value 
		nWidthPrint = nWidthAllowed;
		nHeightPrint = nWidthPrint * nHeightFactor / nWidthFactor;
	}
	else
	{	// Use the maximum height, and calculate the width value 
		nHeightPrint = nHeightAllowed;
		nWidthPrint = nHeightPrint * nWidthFactor / nHeightFactor;
	}

	// Start the print job 
	if ( StartDoc( PrinterDC, &DocInfo ) <= 0 )
	{
		DeleteDC( PrinterDC );
		return false;
	}
	StartPage( PrinterDC );

	// Print image
	switch ( nDevide )
	{
	case 4:
	case 9:
	case 16:
		{
			int	n, x, y, cx, cy, m;
			n = (int)sqrt( (double)nDevide );	// devide number
			m = nWidthPrint/40;					// margin
			cx = (nWidthPrint - m * 2)/n;
			cy = (nHeightPrint - m * 2)/n;
			for ( y = 0; y < n; y++ )
			{
				for ( x = 0; x < n; x++ )
				{
					L_PrintBitmapFast( PrinterDC, &LBitmap, x * cx + m, y * cy + m, cx, cy, FALSE );
				}
			}

			// 재단선 그리기
			HPEN	hOldPen;
			hOldPen = (HPEN)SelectObject( PrinterDC, GetStockObject( BLACK_PEN ) );
			for ( x = 0; x <= n; x++ )
			{
				MoveToEx( PrinterDC, x * cx + m, 0, NULL );
//				LineTo( PrinterDC, x * cx + m, m );
//				MoveToEx( PrinterDC, x * cx + m, nHeightPrint - m, NULL );
				LineTo( PrinterDC, x * cx + m, nHeightPrint );
			}
			for ( y = 0; y <= n; y++ )
			{
				MoveToEx( PrinterDC, 0, y * cy + m, NULL );
//				LineTo( PrinterDC, m, y * cy + m );
//				MoveToEx( PrinterDC, nWidthPrint - m, y * cy + m, NULL );
				LineTo( PrinterDC, nWidthPrint, y * cy + m );
			}
			SelectObject( PrinterDC, hOldPen );
		}
		break;
	default:
		L_PrintBitmapFast( PrinterDC, &LBitmap, 1, 1, nWidthPrint, nHeightPrint, FALSE );
		break;
	}

	L_FreeBitmap( &LBitmap );

	// Flush the page and finish the print job
	EndPage( PrinterDC );
	EndDoc( PrinterDC );
	DeleteDC( PrinterDC );

	return true;
*/
	return false;
}

HRGN	DibCreateRegion( LPBYTE lpDib, BYTE byColor )
{
	LPBYTE			lpData;
	int				x, y, cx, cy;
	DWORD			dwMaxRect;
	RECT			r;
	RGNDATA			*pRd;
	HGLOBAL			hMem;
	HRGN			hRgn;

	if ( !lpDib || DIB_BPP( lpDib ) != 8 )
		return NULL;

	cx = DIB_CX( lpDib );
	cy = DIB_CY( lpDib );
	dwMaxRect = 3000;

	hMem = GlobalAlloc( GMEM_FIXED, sizeof(RGNDATAHEADER) + sizeof(RECT) * dwMaxRect );
	pRd = (RGNDATA *)GlobalLock( hMem );
	pRd->rdh.dwSize = sizeof(RGNDATAHEADER);
	pRd->rdh.iType = RDH_RECTANGLES;
	pRd->rdh.nCount = 0;
	pRd->rdh.nRgnSize = 0;
	SetRect( &(pRd->rdh.rcBound), cx, cy, 0, 0 );

	for ( y = 0; y < cy; y++ )
	{
		lpData = DIB_DATA8XY_INV( lpDib, 0, y );
		for ( x = 0; x < cx; x++ )
		{
			if ( *lpData == byColor )
			{
				// get run length rect
				r.left = x;
				r.top = r.bottom = y;
				while ( *lpData == byColor && x < cx )
				{
					x++;
					lpData++;
				}
				r.right = x;

				// update bound rect
				if ( r.left < pRd->rdh.rcBound.left )
					pRd->rdh.rcBound.left = r.left;
				if ( r.top < pRd->rdh.rcBound.top )
					pRd->rdh.rcBound.top = r.top;
				if ( r.right > pRd->rdh.rcBound.right )
					pRd->rdh.rcBound.right = r.right;
				if ( r.bottom > pRd->rdh.rcBound.bottom )
					pRd->rdh.rcBound.bottom = r.bottom;

				memcpy( &pRd->Buffer[pRd->rdh.nCount++], &r, sizeof(RECT) );
				if ( pRd->rdh.nCount >= dwMaxRect )
					goto exitLoop;
			}
			lpData++;
		}
	}

exitLoop:

	pRd->rdh.nRgnSize = sizeof(RECT) * pRd->rdh.nCount;
	hRgn = ExtCreateRegion( NULL, sizeof(RGNDATAHEADER) + sizeof(RECT) * pRd->rdh.nCount, pRd );

	GlobalUnlock( hMem );
	GlobalFree( hMem );

	return hRgn;
}

bool	DibAlphaMasking( LPBYTE lpDstDib, LPBYTE lpSrcDib, LPBYTE lpMaskDib )
{
	if ( !lpDstDib || !lpSrcDib || !lpMaskDib || 
		DIB_BPP( lpSrcDib ) != DIB_BPP( lpMaskDib ) ||
		DIB_BPP( lpDstDib ) != DIB_BPP( lpMaskDib ) ||
		DIB_BPP( lpMaskDib ) != 8 )
		return false;

	LPBYTE	lpSrc, lpDst, lpMask;
	int		x, y, cx, cy;
	cx = DIB_CX( lpMaskDib );
	cy = DIB_CY( lpMaskDib );

	if ( cx != DIB_CX( lpSrcDib ) || cy != DIB_CY( lpSrcDib ) ||
		cx != DIB_CX( lpDstDib ) || cy != DIB_CY( lpDstDib ) )
		return false;

	for ( y = 0; y < cy; y++ )
	{
		lpSrc = DIB_DATAXY( lpSrcDib, 0, y );
		lpDst = DIB_DATAXY( lpDstDib, 0, y );
		lpMask = DIB_DATAXY( lpMaskDib, 0, y );
		for ( x = 0; x < cx; x++ )
		{
			*lpDst += (255 - *lpSrc) * *lpMask / 255;
			lpSrc++;
			lpDst++;
			lpMask++;
		}
	}

	return true;
}


////////////////////////////////////////////////////////////////////////////////
//	filter

bool	DibInvert( LPBYTE lpSrcDib )
{
	LPBYTE	lpSrc;
	int		nSize, i;

	if ( !lpSrcDib ) return false;

	nSize = DIB_DATASIZE( lpSrcDib );
	lpSrc = DIB_DATA( lpSrcDib );
	for ( i = 0; i < nSize; i++, lpSrc++ )
		*lpSrc = 0xff - *lpSrc;

	return true;
}
