// DIB.H
// Made by ±èÅÂÇü(gapacci@hotmail.com)
//

#ifndef _DIB_H_
#define _DIB_H_

#define ALIGN_2B( a )				((a)%2 ? (a) + 1 : (a))
#define ALIGN_4B( a )				((a)%4 ? (a) + 4 - ((a)%4) : (a))

#define DIB_DATA( a )				((LPBYTE)(a) + sizeof(BITMAPINFOHEADER) + DIB_PALSIZE(a))
#define DIB_HEADER( a )				((LPBITMAPINFOHEADER)(a))
#define DIB_BMI( a )				((LPBITMAPINFO)(a))
#define DIB_CX( a )					(DIB_HEADER(a)->biWidth)
#define DIB_CY( a )					(DIB_HEADER(a)->biHeight)
#define DIB_BPP( a )				(DIB_HEADER(a)->biBitCount)
#define DIB_PALSIZE( a )			(DIB_BPP(a) == 24 ? 0 : sizeof(RGBQUAD) * (1 << DIB_BPP(a)))
#define DIB_LINESIZE( a )			(ALIGN_4B(DIB_CX(a) * DIB_BPP(a) / 8))
#define DIB_DATASIZE( a )			(DIB_LINESIZE(a) * DIB_CY(a))
#define DIB_HEADERSIZE( a )			(sizeof(BITMAPINFOHEADER))
#define DIB_SIZE( a )				(DIB_HEADERSIZE(a) + DIB_PALSIZE(a) + DIB_DATASIZE(a))
#define DIB_DATAXY( a, x, y )		(DIB_DATA(a) + (x) * DIB_BPP(a)/8 + (y) * ALIGN_4B( DIB_CX(a) * DIB_BPP(a)/8 ))
#define DIB_DATAXY_INV( a, x, y )	(DIB_DATA(a) + (x) * DIB_BPP(a)/8 + (DIB_CY(a) - (y) - 1) * ALIGN_4B( DIB_CX(a) * DIB_BPP(a)/8 ))
#define DIB_DATA8XY( a, x, y )		(DIB_DATA(a) + (x) + (y) * ALIGN_4B( DIB_CX(a) ))
#define DIB_DATA24XY( a, x, y )		(DIB_DATA(a) + (x) * 3 + (y) * ALIGN_4B( DIB_CX(a) * 3 ))
#define DIB_DATA8XY_INV( a, x, y )	(DIB_DATA(a) + (x) + (DIB_CY(a) - (y) - 1) * ALIGN_4B( DIB_CX(a) ))
#define DIB_DATA24XY_INV( a, x, y ) (DIB_DATA(a) + (x) * 3 + (DIB_CY(a) - (y) - 1) * ALIGN_4B( DIB_CX(a) * 3 ))

#define DIB_ALPHAXY( a, x, y )		DIB_DATA8XY( (a), (x), (y) )
#define DIB_IMAGEXY( a, x, y )		DIB_DATA24XY( (a), (x), (y) )
#define DIB_ALPHAXY_INV( a, x, y )	DIB_DATA8XY_INV( (a), (x), (y) )
#define DIB_IMAGEXY_INV( a, x, y )	DIB_DATA24XY_INV( (a), (x), (y) )

// file io
extern LPBYTE	DibLoadHandle( const char* lpFileName );
extern bool		DibSave( LPBYTE lpDib, const char* lpFileName );

// creation
extern LPBYTE	DibCreateEmpty( int nBitsPerPixel, int nWidth, int nHeight );
extern LPBYTE	DibCreateEmptyHandle( int nBitsPerPixel, int nWidth, int nHeight );
extern void		DibDeleteHandle( LPBYTE lpSrc );

// copy
extern LPBYTE	DibDuplicate( LPBYTE lpSrc );
extern LPBYTE	DibDuplicateHandle( LPBYTE lpSrc );
extern bool		DibCopyRect( LPBYTE lpDstDib, int dstx, int dsty, LPBYTE lpSrcDib, LPRECT prcSrc );
extern bool		DibCopyRectROP( LPBYTE lpDstDib, int dstx, int dsty, LPBYTE lpSrcDib, LPRECT prcSrc, DWORD dwROP );
extern bool		DibCopy( LPBYTE lpDst, LPBYTE lpSrc );
extern bool		DibTile( LPBYTE lpSrcDib, LPBYTE lpTileDib );

// geometry
extern LPBYTE	DibFlipHandle( LPBYTE lpDib );
extern LPBYTE	DibReverseHandle( LPBYTE lpDib );
extern LPBYTE	DibRotateHandle( LPBYTE lpDib, float fDegree );
extern LPBYTE	DibResizeHandle( LPBYTE lpDib, int nNewWidth, int nNewHeight );
extern LPBYTE	DibReduce( LPBYTE lpSrcDib, int newCx, int newCy );

// convert format
extern LPBYTE	DibRGB2GRAYHandle( LPBYTE lpRgbDib );
extern LPBYTE	DibGRAY2RGBHandle( LPBYTE lpGrayDib );

// etc
extern bool		DibPrint( HWND hwnd, LPBYTE lpDib, const char* lpszDevice, int nDevide );
extern HRGN		DibCreateRegion( LPBYTE lpDib, BYTE byColor );
extern bool		DibAlphaMasking( LPBYTE lpDstDib, LPBYTE lpSrcDib, LPBYTE lpMaskDib );

// filter
extern bool		DibInvert( LPBYTE lpSrcDib );

#endif
