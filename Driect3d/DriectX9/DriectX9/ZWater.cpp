#include "ZWater.h"

ZWater::ZWater()
{
	m_pDev = NULL;
	m_pVB = NULL;
	m_pIB = NULL;
	m_cx = m_cz = 0;
	m_nWaveTick = 0;
	m_fWave = 0.0f;
	m_fDir = 0.1f;
	m_nPrevTick = GetTickCount();
	m_pTexWater[0] = NULL;
	m_pTexWater[1] = NULL;
}

ZWater::~ZWater()
{
	S_REL( m_pTexWater[0] );
	S_REL( m_pTexWater[1] );
	S_REL( m_pVB );
}

// 물메시 생성
int ZWater::Create( LPDIRECT3DDEVICE9 pDev, int cx, int cz, int nSpeed )
{
	m_pDev = pDev;
	m_cx = cx;
	m_cz = cz;
	m_nWaveTick = nSpeed;

	_LoadTextures();
	_CreateVIB();

	return 1;
}

// 물표현에 사용될 텍스처 2장
int ZWater::_LoadTextures()
{
	D3DXCreateTextureFromFile( m_pDev, "water1.bmp", &m_pTexWater[0] );
	D3DXCreateTextureFromFile( m_pDev, "water2.bmp", &m_pTexWater[1] );

	return 1;
}

// 4각형 정점버퍼를 만든다
// 지금은 4각형 메시를 여러번 출력하는데, 이것은 무지무지 비효율적인 방식이다
// 물전체를 표현할수 있는 커다란 정점버퍼(혹은 메시)를 만드는것이 더 낫다
int ZWater::_CreateVIB()
{
	WATERVERTEX vtx[4] =
	{
		{ -0.5f, 0.0f, -0.5f, 0x7fffffff, 0.0f, 1.0f },
		{ -0.5f, 0.0f,  0.5f, 0x7fffffff, 0.0f, 0.0f },
		{  0.5f, 0.0f, -0.5f, 0x7fffffff, 1.0f, 1.0f },
		{  0.5f, 0.0f,  0.5f, 0x7fffffff, 1.0f, 0.0f }
	};

	if( FAILED( m_pDev->CreateVertexBuffer( 4 * sizeof(WATERVERTEX),
										    0, WATERVERTEX::FVF,
                                            D3DPOOL_DEFAULT, &m_pVB, NULL ) ) )
		return 0;

	WATERVERTEX* pV;

	if( FAILED( m_pVB->Lock( 0, 4 * sizeof(WATERVERTEX), (void**)&pV, 0 ) ) )
		return E_FAIL;

	memcpy( pV, vtx, 4 * sizeof(WATERVERTEX) );
	m_pVB->Unlock();

    return 1;
}

// 텍스처 애니메이션
int ZWater::_WaveTexture()
{
	D3DXMATRIXA16	matTex;
	D3DXMatrixIdentity( &matTex );

	int nCurrTick = GetTickCount();
	if( (nCurrTick - m_nPrevTick) > m_nWaveTick )
	{
		// 텍스처를 애니메이션 시킨다
		m_fWave += m_fDir;
		if( m_fWave > (D3DX_PI/8.0f) ) m_fDir = -0.01f;
		if( m_fWave < (-D3DX_PI/8.0f) ) m_fDir = 0.01f;

		// 0번 스테이지의 텍스처는 텍스처변환 행렬(D3DTS_TEXTURE0)과 곱해지도록 한다.
		m_pDev->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );
		// 1번 스테이지의 텍스처는 텍스처변환 행렬(D3DTS_TEXTURE1)과 곱해지도록 한다.
		m_pDev->SetTextureStageState( 1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2 );

		// 0번 텍스처 변환행렬 설정
		D3DXMatrixRotationX( &matTex, m_fWave );
		m_pDev->SetTransform( D3DTS_TEXTURE0, &matTex );
		
		// 1번 텍스처 변환행렬 설정
		D3DXMatrixRotationY( &matTex, m_fWave );
		m_pDev->SetTransform( D3DTS_TEXTURE1, &matTex );
	}
	return 1;
}

int ZWater::Draw()
{
	float x, z;
	D3DXMATRIXA16	matWorld;
	D3DXMatrixIdentity( &matWorld );

	_WaveTexture();
	m_pDev->SetTexture( 0, m_pTexWater[0] );
	m_pDev->SetTexture( 1, m_pTexWater[1] );
	m_pDev->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );		// 0번 텍스처 : 0번 텍스처 인덱스 사용
	m_pDev->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 0 );		// 1번 텍스처 : 0번 텍스처 인덱스 사용
	m_pDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pDev->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_pDev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	m_pDev->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	m_pDev->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT );
	m_pDev->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_MIRROR );
	m_pDev->SetSamplerState( 1, D3DSAMP_ADDRESSV, D3DTADDRESS_MIRROR );
	m_pDev->SetStreamSource( 0, m_pVB, 0, sizeof(WATERVERTEX) );
	m_pDev->SetFVF( WATERVERTEX::FVF );

	// 루프를 돌며 메시를 그린다.
	// 앞서 밝혔지만, 메시를 그리는 최악의 알고리즘이다.
	// 효율좋게 바꾸는것이 좋다.
	// 나는 귀차니즘으로 그냥 넘어간다.
	for( z = 0.0f ; z < (float)m_cz ; z++ )
	{
		for( x = 0.0f ; x < (float)m_cx ; x++ )
		{
			matWorld._41 = x - m_cx/2.0f;
			matWorld._43 = -((float)z-m_cz/2.0f);
			m_pDev->SetTransform( D3DTS_WORLD, &matWorld );
			m_pDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
		}
	}

	D3DXMatrixIdentity( &matWorld );
	m_pDev->SetTransform( D3DTS_WORLD, &matWorld );
	m_pDev->SetTransform( D3DTS_TEXTURE0, &matWorld );
	m_pDev->SetTransform( D3DTS_TEXTURE1, &matWorld );

	return 1;
}
