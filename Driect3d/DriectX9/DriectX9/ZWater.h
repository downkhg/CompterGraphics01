#ifndef _ZWATER_H_
#define _ZWATER_H_

#include "define.h"

class ZWater
{
	LPDIRECT3DDEVICE9       m_pDev;		// �������� ���� D3D����̽�
	LPDIRECT3DVERTEXBUFFER9 m_pVB;		// ������ ������ ��������
	LPDIRECT3DINDEXBUFFER9	m_pIB;		// �ε����� ������ �ε�������
	LPDIRECT3DTEXTURE9		m_pTexWater[2];// ���� ǥ����Texture
	int						m_cx;		// ���� xũ��
	int						m_cz;		// ���� zũ��
	int						m_nWaveTick;// m_nWaveTick�� �ѹ��� �ִϸ��̼� �Ѵ�
	int						m_nPrevTick;
	float					m_fWave;	// �ؽ�ó�� �̵���ų ��
	float					m_fDir;		// �ؽ�ó�� �̵�����

	struct WATERVERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1 };
		float			px, py, pz;
		DWORD			d;
		float			tx, ty;
	};

protected:
	int	_LoadTextures();	// �ؽ�ó�� �ε��Ѵ�
	int _CreateVIB();		// �������۸� �����
	int _WaveTexture();		// �ؽ�ó�� �ִϸ��̼� �Ѵ�

public:
	ZWater();
	~ZWater();
	int Create( LPDIRECT3DDEVICE9 pDev, int cx, int cz, int nSpeed );
	int Draw();				// �׸���... -_-
};

#endif // _ZWATER_H_