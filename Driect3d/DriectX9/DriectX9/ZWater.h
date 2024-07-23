#ifndef _ZWATER_H_
#define _ZWATER_H_

#include "define.h"

class ZWater
{
	LPDIRECT3DDEVICE9       m_pDev;		// 렌더링에 사용될 D3D디바이스
	LPDIRECT3DVERTEXBUFFER9 m_pVB;		// 정점을 보관할 정점버퍼
	LPDIRECT3DINDEXBUFFER9	m_pIB;		// 인덱스를 보관할 인덱스버퍼
	LPDIRECT3DTEXTURE9		m_pTexWater[2];// 물을 표현할Texture
	int						m_cx;		// 물의 x크기
	int						m_cz;		// 물의 z크기
	int						m_nWaveTick;// m_nWaveTick에 한번씩 애니메이션 한다
	int						m_nPrevTick;
	float					m_fWave;	// 텍스처를 이동시킬 값
	float					m_fDir;		// 텍스처의 이동방향

	struct WATERVERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
		float			px, py, pz;
		DWORD			d;
		float			tx, ty;
	};

protected:
	int	_LoadTextures();	// 텍스처를 로딩한다
	int _CreateVIB();		// 정점버퍼를 만든다
	int _WaveTexture();		// 텍스처를 애니메이션 한다

public:
	ZWater();
	~ZWater();
	int Create( LPDIRECT3DDEVICE9 pDev, int cx, int cz, int nSpeed );
	int Draw();				// 그린다... -_-
};

#endif // _ZWATER_H_