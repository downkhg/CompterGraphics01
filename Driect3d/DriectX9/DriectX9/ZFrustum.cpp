#include "ZFrustum.h"
#include "ZFLog.h"

ZFrustum::ZFrustum()
{
	ZeroMemory( m_vtx, sizeof( m_vtx[0] ) * 8 );
	ZeroMemory( m_plane, sizeof( m_plane[0] ) * 6 );
}

// ī�޶�(view) * ��������(projection)����� �Է¹޾� 6���� ����� �����.
BOOL ZFrustum::Make( D3DXMATRIXA16* pmatViewProj )
{
	int				i;
	D3DXMATRIXA16	matInv;

	// ������ı��� ��ġ�� ��� 3���� ������ǥ�� ���� (-1,-1,0) ~ (1,1,1)������ ������ �ٲ��.
	// m_vtx�� �� ���������� ��谪�� �־�д�.
	m_vtx[0].x = -1.0f;	m_vtx[0].y = -1.0f;	m_vtx[0].z = 0.0f;
	m_vtx[1].x =  1.0f;	m_vtx[1].y = -1.0f;	m_vtx[1].z = 0.0f;
	m_vtx[2].x =  1.0f;	m_vtx[2].y = -1.0f;	m_vtx[2].z = 1.0f;
	m_vtx[3].x = -1.0f;	m_vtx[3].y = -1.0f;	m_vtx[3].z = 1.0f;
	m_vtx[4].x = -1.0f;	m_vtx[4].y =  1.0f;	m_vtx[4].z = 0.0f;
	m_vtx[5].x =  1.0f;	m_vtx[5].y =  1.0f;	m_vtx[5].z = 0.0f;
	m_vtx[6].x =  1.0f;	m_vtx[6].y =  1.0f;	m_vtx[6].z = 1.0f;
	m_vtx[7].x = -1.0f;	m_vtx[7].y =  1.0f;	m_vtx[7].z = 1.0f;

	// view * proj�� ������� ���Ѵ�.
	D3DXMatrixInverse(&matInv, NULL, pmatViewProj );

	// Vertex_���� = Vertex_local * Matrix_world * Matrix_view * Matrix_Proj �ε�,
	// Vertex_world = Vertex_local * Matrix_world�̹Ƿ�,
	// Vertex_���� = Vertex_world * Matrix_view * Matrix_Proj �̴�.
	// Vertex_���� = Vertex_world * ( Matrix_view * Matrix_Proj ) ����
	// �����( Matrix_view * Matrix_Proj )^-1�� �纯�� ���ϸ�
	// Vertex_���� * �����( Matrix_view * Matrix_Proj )^-1 = Vertex_World �� �ȴ�.
	// �׷��Ƿ�, m_vtx * matInv = Vertex_world�� �Ǿ�, ������ǥ���� �������� ��ǥ�� ���� �� �ִ�.
	for( i = 0; i < 8; i++ )
		D3DXVec3TransformCoord( &m_vtx[i], &m_vtx[i], &matInv );

	// 0���� 5���� ���������� near����� ������ܰ� �����ϴ��̹Ƿ�, ���� ��ǥ�� ���ؼ� 2�� ������
	// ī�޶��� ��ǥ�� ���� �� �ִ�.(��Ȯ�� ��ġ�ϴ� ���� �ƴϴ�.)
	m_vPos = ( m_vtx[0] + m_vtx[5] ) / 2.0f;
	
	// ����� ������ǥ�� �������� ����� �����
	// ���Ͱ� �������� ���ʿ��� �ٱ������� ������ �����̴�.
//	D3DXPlaneFromPoints(&m_plane[0], m_vtx+4, m_vtx+7, m_vtx+6);	// �� ���(top)
//	D3DXPlaneFromPoints(&m_plane[1], m_vtx  , m_vtx+1, m_vtx+2);	// �� ���(bottom)
//	D3DXPlaneFromPoints(&m_plane[2], m_vtx  , m_vtx+4, m_vtx+5);	// �� ���(near)
	D3DXPlaneFromPoints(&m_plane[3], m_vtx+2, m_vtx+6, m_vtx+7);	// �� ���(far)
	D3DXPlaneFromPoints(&m_plane[4], m_vtx  , m_vtx+3, m_vtx+7);	// �� ���(left)
	D3DXPlaneFromPoints(&m_plane[5], m_vtx+1, m_vtx+5, m_vtx+6);	// �� ���(right)

	return TRUE;
}

/// ���� v�� �������Ҿȿ� ������ TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
BOOL ZFrustum::IsIn( D3DXVECTOR3* pv )
{
	float		fDist;
//	int			i;

	// ����� left, right, far plane�� �����Ѵ�.
//	for( i = 0 ; i < 6 ; i++ )
	{
		fDist = D3DXPlaneDotCoord( &m_plane[3], pv );
		if( fDist > PLANE_EPSILON ) return FALSE;	// plane�� normal���Ͱ� far�� ���ϰ� �����Ƿ� ����̸� ���������� �ٱ���
		fDist = D3DXPlaneDotCoord( &m_plane[4], pv );
		if( fDist > PLANE_EPSILON ) return FALSE;	// plane�� normal���Ͱ� left�� ���ϰ� �����Ƿ� ����̸� ���������� ����
		fDist = D3DXPlaneDotCoord( &m_plane[5], pv );
		if( fDist > PLANE_EPSILON ) return FALSE;	// plane�� normal���Ͱ� right�� ���ϰ� �����Ƿ� ����̸� ���������� ������
	}

	return TRUE;
}

/** �߽�(v)�� ������(radius)�� ���� ��豸(bounding sphere)�� �������Ҿȿ� ������
 *  TRUE�� ��ȯ, �ƴϸ� FALSE�� ��ȯ�Ѵ�.
 */
BOOL ZFrustum::IsInSphere( D3DXVECTOR3* pv, float radius )
{
	float		fDist;

	fDist = D3DXPlaneDotCoord( &m_plane[3], pv );
	if( fDist > (radius+PLANE_EPSILON) ) return FALSE;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
	fDist = D3DXPlaneDotCoord( &m_plane[4], pv );
	if( fDist > (radius+PLANE_EPSILON) ) return FALSE;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����
	fDist = D3DXPlaneDotCoord( &m_plane[5], pv );
	if( fDist > (radius+PLANE_EPSILON) ) return FALSE;	// ���� �߽����� �Ÿ��� ���������� ũ�� �������ҿ� ����

	return TRUE;
}

/// ���������� ȭ�鿡 �׷��ش�.
BOOL ZFrustum::Draw( LPDIRECT3DDEVICE9 pDev )
{
	WORD		index[] = { 0, 1, 2,
							0, 2, 3,
							4, 7, 6,
							4, 6, 5,
							1, 5, 6,
							1, 6, 2,
							0, 3, 7,
							0, 7, 4,
							0, 4, 5,
							0, 5, 1,
							3, 7, 6,
							3, 6, 2 };

    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );

	typedef struct tagVTX
	{
		D3DXVECTOR3	p;
	} VTX;

	VTX		vtx[8];

	for( int i = 0 ; i < 8 ; i++ )
		vtx[i].p = m_vtx[i];

	pDev->SetFVF( D3DFVF_XYZ );
	pDev->SetStreamSource( 0, NULL, 0, sizeof(VTX) );
	pDev->SetTexture( 0, NULL );
	pDev->SetIndices( 0 );
	pDev->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	pDev->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	pDev->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	pDev->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
	pDev->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );

	// �Ķ������� ��,�� ����� �׸���.
    pDev->SetRenderState( D3DRS_LIGHTING, TRUE );
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    pDev->SetMaterial( &mtrl );
	pDev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	// ������� ��,�� ����� �׸���.
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    pDev->SetMaterial( &mtrl );
	pDev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index+4*3, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	// ���������� ��,�� ����� �׸���.
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    pDev->SetMaterial( &mtrl );
	pDev->DrawIndexedPrimitiveUP( D3DPT_TRIANGLELIST, 0, 8, 4, index+8*3, D3DFMT_INDEX16, vtx, sizeof( vtx[0] ) );

	pDev->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
    pDev->SetRenderState( D3DRS_LIGHTING, FALSE );
	return TRUE;
}

void ZMakePlane( D3DXPLANE* pPlane, D3DXVECTOR3* pv0, D3DXVECTOR3* pv1, D3DXVECTOR3* pv2 )
{
	D3DXPlaneFromPoints( pPlane, pv0, pv1, pv2 );
//  ����� �������� ���� �����ϴ� �ҽ�
//	for OpenGL
//	D3DXVECTOR3	v0, v1, v2;
//	v1 = *pv1 - *pv0;
//	v2 = *pv2 - *pv0;
//	D3DXVec3Cross( &v0, &v1, &v2 );
//	D3DXVec3Normalize( &v0, &v0 );

//	pPlane->a = v0.x;
//	pPlane->b = v0.y;
//	pPlane->c = v0.z;
//	pPlane->d = -( v0.x * pv0->x + v0.y * pv0->y + v0.z * pv0->z );
}