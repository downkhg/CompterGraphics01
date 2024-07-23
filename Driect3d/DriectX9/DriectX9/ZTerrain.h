#ifndef _ZTERRAIN_H_
#define _ZTERRAIN_H_

#include "define.h"
#include "ZQuadTree.h"
#define MAX_TERRAIN_TEX	4

/**
 * @brief �������� Ŭ����
 * 
 */
class ZTerrain
{
private:
	int						m_cxDIB;		/// DIB�� �����ȼ���
	int						m_czDIB;		/// DIB�� �����ȼ���
	int						m_cyDIB;		/// DIB�� �ִ���̰�(�� 0 ~ 255������ ��)
	D3DXVECTOR3				m_vfScale;		/// x scale, y scale, z scale
	TERRAINVERTEX*			m_pvHeightMap;	/// ���̸��� �����迭
	LPDIRECT3DDEVICE9		m_pd3dDevice;	/// ��¿� D3D����̽�
	LPDIRECT3DTEXTURE9		m_pTex[MAX_TERRAIN_TEX];	/// �ؽ�ó
	LPDIRECT3DVERTEXBUFFER9	m_pVB;			/// ������¿� ��������
	LPDIRECT3DINDEXBUFFER9	m_pIB;			/// ������¿� �ε�������
	int						m_nTriangles;	/// ����� �ﰢ���� ����
	ZQuadTree*				m_pQuadTree;	/// ����Ʈ�� ��ü�� ������
	float					m_fLODRatio;	/// LODó���ÿ� ���� ��

public:
	// ������ ���õ� �Լ���
				ZTerrain();
				~ZTerrain();
	/**
	 * @brief ������ü�� �ʱ�ȭ�Ѵ�.
	 * @param pDev : ��¿� D3D����̽� 
	 * @param pvfScale : ���̸��� x,y,z���� ���� ô����(scale vector)
	 * @param fLODRatio : LODó���ÿ� ���� ��
	 * @param lpBMPFilename : ���̸ʿ� BMP���ϸ�
	 * @param lpTexFilename : ��¿� D3D����̽� 
	 */
	HRESULT		Create( LPDIRECT3DDEVICE9 pDev, D3DXVECTOR3* pvfScale, float fLODRatio, const char* lpBMPFilename,const char* lpTexFilename[MAX_TERRAIN_TEX] );

	// ZTerrain���ο��� ���Ǵ� �Լ���
private:
	/// ������ü�� �޸𸮿��� �Ұ��Ѵ�.
	HRESULT		_Destroy();

	/// ������ü���� ����� �ؽ�ó�� �о���δ�.
	HRESULT		_LoadTextures( const char* lpTexFilename[MAX_TERRAIN_TEX] );

	/// BMP������ ��� ���̸��� �����Ѵ�.
	HRESULT		_BuildHeightMap( const char* lpFilename );

	/// BMP������ ũ�⿡ ���缭 ����Ʈ���� �����Ѵ�.
	HRESULT		_BuildQuadTree();

	/// ����, �ε��� ���۸� �����Ѵ�.
	HRESULT		_CreateVIB();

	/// ȭ�鿡 ������ ����Ѵ�.
	HRESULT		_Render();
public:
	/// x, z��ġ�� �������� ����.
	TERRAINVERTEX*	GetVertex( int x, int z ) { return (m_pvHeightMap+x+z*m_cxDIB); }

	/// x, z��ġ�� ����(y)���� ����.
	float		GetHeight( int x, int z ) { return (GetVertex( x, z ))->p.y; }

	/// ���̸� BMP������ �����ȼ���
	int			GetCXDIB() { return m_cxDIB; }

	/// ���̸� BMP������ �����ȼ���
	int			GetCZDIB() { return m_czDIB; }

	/// ���̸��� ���� x�� ��(column)��
	float		GetCXTerrain() { return m_cxDIB * m_vfScale.x; }

	/// ���̸��� ���� y�� ����(height)��
	float		GetCYTerrain() { return m_cyDIB * m_vfScale.y; }

	/// ���̸��� ���� z�� ����(row)��
	float		GetCZTerrain() { return m_czDIB * m_vfScale.z; }

	/// ȭ�鿡 ������ ����Ѵ�.
	HRESULT		Draw( ZFrustum* pFrustum );
};

#endif // _ZTERRAIN_H_