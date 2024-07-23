#include "ZQuadTree.h"

extern LPD3DXFONT g_pFont;

// ���� ��Ʈ��� ������
ZQuadTree::ZQuadTree( int cx, int cy )
{
	int		i;
	m_pParent = NULL;
	for( i = 0 ; i < 4 ; i++ )
	{
		m_pChild[i] = NULL;
		m_pNeighbor[i] = NULL;
	}

	// ��Ʈ����� 4�� �ڳʰ� ����
	m_nCorner[CORNER_TL]	= 0;
	m_nCorner[CORNER_TR]	= cx - 1;
	m_nCorner[CORNER_BL]	= cx * ( cy - 1 );
	m_nCorner[CORNER_BR]	= cx * cy - 1;
	m_nCenter				= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + 
								m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 4;

	m_bCulled = FALSE;
	m_fRadius = 0.0f;
}

// ���� �ڽĳ�� ������
ZQuadTree::ZQuadTree( ZQuadTree* pParent )
{
	int		i;
	m_pParent = pParent;
	m_nCenter = 0;
	for( i = 0 ; i < 4 ; i++ )
	{
		m_pChild[i] = NULL;
		m_pNeighbor[i] = NULL;
		m_nCorner[i] = 0;
	}

	m_bCulled = FALSE;
	m_fRadius = 0.0f;
}

// �Ҹ���
ZQuadTree::~ZQuadTree()
{
	_Destroy();
}

// �޸𸮿��� ����Ʈ���� �����Ѵ�.
void	ZQuadTree::_Destroy()
{
	// �ڽ� ������ �Ҹ� ��Ų��.
	for( int i = 0 ; i < 4 ; i++ ) DEL( m_pChild[i] );
}

// 4���� �ڳʰ��� �����Ѵ�.
BOOL	ZQuadTree::_SetCorners( int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR )
{
	m_nCorner[CORNER_TL] = nCornerTL;
	m_nCorner[CORNER_TR] = nCornerTR;
	m_nCorner[CORNER_BL] = nCornerBL;
	m_nCorner[CORNER_BR] = nCornerBR;
	m_nCenter			 = ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + 
							 m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 4;
	return TRUE;
}

// �ڽ� ��带 �߰��Ѵ�.
ZQuadTree*	ZQuadTree::_AddChild( int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR )
{
	ZQuadTree*	pChild;

	pChild = new ZQuadTree( this );
	pChild->_SetCorners( nCornerTL, nCornerTR, nCornerBL, nCornerBR );

	return pChild;
}

// Quadtree�� 4���� �ڽ� Ʈ���� �κк���(subdivide)�Ѵ�.
BOOL	ZQuadTree::_SubDivide()
{
	int		nTopEdgeCenter;
	int		nBottomEdgeCenter;
	int		nLeftEdgeCenter;
	int		nRightEdgeCenter;
	int		nCentralPoint;

	// ��ܺ� ���
	nTopEdgeCenter		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] ) / 2;
	// �ϴܺ� ��� 
	nBottomEdgeCenter	= ( m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 2;
	// ������ ���
	nLeftEdgeCenter		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL] ) / 2;
	// ������ ���
	nRightEdgeCenter	= ( m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR] ) / 2;
	// �Ѱ��
	nCentralPoint		= ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] + 
							m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 4;

	// ���̻� ������ �Ұ����Ѱ�? �׷��ٸ� SubDivide() ����
	if( (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL]) <= 1 )
	{
		return FALSE;
	}

	// 4���� �ڽĳ�� �߰�
	m_pChild[CORNER_TL] = _AddChild( m_nCorner[CORNER_TL], nTopEdgeCenter, nLeftEdgeCenter, nCentralPoint );
	m_pChild[CORNER_TR] = _AddChild( nTopEdgeCenter, m_nCorner[CORNER_TR], nCentralPoint, nRightEdgeCenter );
	m_pChild[CORNER_BL] = _AddChild( nLeftEdgeCenter, nCentralPoint, m_nCorner[CORNER_BL], nBottomEdgeCenter );
	m_pChild[CORNER_BR] = _AddChild( nCentralPoint, nRightEdgeCenter, nBottomEdgeCenter, m_nCorner[CORNER_BR] );
	
	return TRUE;
}

/*
void	ZQuadTree::_PrintLOD( TERRAINVERTEX* pHeightMap, ZFrustum* pFrustum, float fLODRatio )
{
	if( IsVisible( pHeightMap, pFrustum->GetPos(), fLODRatio ) )
	{
	}

	if( m_pChild[0] )
	{
		m_pChild[0]->_PrintLOD( pHeightMap, pFrustum, fLODRatio )
		m_pChild[1]->_PrintLOD( pHeightMap, pFrustum, fLODRatio )
		m_pChild[2]->_PrintLOD( pHeightMap, pFrustum, fLODRatio )
		m_pChild[3]->_PrintLOD( pHeightMap, pFrustum, fLODRatio )
	}

	return;
}
*/

// ����� �������� �ε����� �����Ѵ�.
int		ZQuadTree::_GenTriIndex( int nTris, LPVOID pIndex, TERRAINVERTEX* pHeightMap, ZFrustum* pFrustum, float fLODRatio )
{
	// �ø��� ����� �׳� ����
	if( m_bCulled )//&& IsVisible( pHeightMap, pFrustum->GetPos(), fLODRatio ) )
	{
//		if( m_pParent )
//			if( m_pParent->IsCulled() )
			{
				m_bCulled = FALSE;
				return nTris;
			}
	}

#ifdef _USE_INDEX16
		LPWORD p = ((LPWORD)pIndex) + nTris * 3;
#else
		LPDWORD p = ((LPDWORD)pIndex) + nTris * 3;
#endif

	// ���� ��尡 ��µǾ�� �ϴ°�?
	if( IsVisible( pHeightMap, pFrustum->GetPos(), fLODRatio ) )
	{
		// ���� ������ ����� �κк���(subdivide)�� �Ұ����ϹǷ� �׳� ����ϰ� �����Ѵ�.
		if( m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= 1 )
		{
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			nTris++;
			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];
			nTris++;
			return nTris;
		}

		BOOL	b[4]={0,0,0,0};
		// ��� �̿����(neightbor node)�� ��°����Ѱ�?
		if( m_pNeighbor[EDGE_UP] ) b[EDGE_UP] = m_pNeighbor[EDGE_UP]->IsVisible( pHeightMap, pFrustum->GetPos(), fLODRatio );
		// �ϴ� �̿����(neightbor node)�� ��°����Ѱ�?
		if( m_pNeighbor[EDGE_DN] ) b[EDGE_DN] = m_pNeighbor[EDGE_DN]->IsVisible( pHeightMap, pFrustum->GetPos(), fLODRatio );
		// ���� �̿����(neightbor node)�� ��°����Ѱ�?
		if( m_pNeighbor[EDGE_LT] ) b[EDGE_LT] = m_pNeighbor[EDGE_LT]->IsVisible( pHeightMap, pFrustum->GetPos(), fLODRatio );
		// ���� �̿����(neightbor node)�� ��°����Ѱ�?
		if( m_pNeighbor[EDGE_RT] ) b[EDGE_RT] = m_pNeighbor[EDGE_RT]->IsVisible( pHeightMap, pFrustum->GetPos(), fLODRatio );

		// �̿������� ��δ� ��°����ϴٸ� ������� �̿������� ���� LOD�̹Ƿ� 
		// �κк����� �ʿ����.
		if( b[EDGE_UP] && b[EDGE_DN] && b[EDGE_LT] && b[EDGE_RT] )
		{
			*p++ = m_nCorner[0];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[2];
			nTris++;
			*p++ = m_nCorner[2];
			*p++ = m_nCorner[1];
			*p++ = m_nCorner[3];
			nTris++;
			return nTris;
		}

		int		n;
		
		if( !b[EDGE_UP] ) // ��� �κк����� �ʿ��Ѱ�?
		{
			n = ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_TR] ) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TL]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TR]; nTris++;
		}
		else	// ��� �κк����� �ʿ���� ���
		{ *p++ = m_nCenter; *p++ = m_nCorner[CORNER_TL]; *p++ = m_nCorner[CORNER_TR]; nTris++; }

		if( !b[EDGE_DN] ) // �ϴ� �κк����� �ʿ��Ѱ�?
		{
			n = ( m_nCorner[CORNER_BL] + m_nCorner[CORNER_BR] ) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BL]; nTris++;
		}
		else	// �ϴ� �κк����� �ʿ���� ���
		{ *p++ = m_nCenter; *p++ = m_nCorner[CORNER_BR]; *p++ = m_nCorner[CORNER_BL]; nTris++; }

		if( !b[EDGE_LT] ) // ���� �κк����� �ʿ��Ѱ�?
		{
			n = ( m_nCorner[CORNER_TL] + m_nCorner[CORNER_BL] ) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_TL]; nTris++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{ *p++ = m_nCenter; *p++ = m_nCorner[CORNER_BL]; *p++ = m_nCorner[CORNER_TL]; nTris++; }

		if( !b[EDGE_RT] ) // ���� �κк����� �ʿ��Ѱ�?
		{
			n = ( m_nCorner[CORNER_TR] + m_nCorner[CORNER_BR] ) / 2;
			*p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = n; nTris++;
			*p++ = m_nCenter; *p++ = n; *p++ = m_nCorner[CORNER_BR]; nTris++;
		}
		else	// ���� �κк����� �ʿ���� ���
		{ *p++ = m_nCenter; *p++ = m_nCorner[CORNER_TR]; *p++ = m_nCorner[CORNER_BR]; nTris++; }
		
		return nTris;	// �� ��� �Ʒ��� �ڽĳ��� Ž���� �ʿ�����Ƿ� ����!
	}

	// �ڽ� ���� �˻�
	if( m_pChild[CORNER_TL] ) nTris = m_pChild[CORNER_TL]->_GenTriIndex( nTris, pIndex, pHeightMap, pFrustum, fLODRatio );
	if( m_pChild[CORNER_TR] ) nTris = m_pChild[CORNER_TR]->_GenTriIndex( nTris, pIndex, pHeightMap, pFrustum, fLODRatio );
	if( m_pChild[CORNER_BL] ) nTris = m_pChild[CORNER_BL]->_GenTriIndex( nTris, pIndex, pHeightMap, pFrustum, fLODRatio );
	if( m_pChild[CORNER_BR] ) nTris = m_pChild[CORNER_BR]->_GenTriIndex( nTris, pIndex, pHeightMap, pFrustum, fLODRatio );

	return nTris;
}

// �����尡 �������ҿ� ���ԵǴ°�?
int ZQuadTree::_IsInFrustum( TERRAINVERTEX* pHeightMap, ZFrustum* pFrustum )
{
	BOOL	b[4];
	BOOL	bInSphere;

	// ��豸�ȿ� �ִ°�?
	bInSphere = pFrustum->IsInSphere( (D3DXVECTOR3*)(pHeightMap+m_nCenter), m_fRadius );
	// ��豸 �ȿ� ������ �������� �������� �׽�Ʈ�� �ʿ�����Ƿ� ����
	if( !bInSphere ) return FRUSTUM_OUT;	

	// ����Ʈ���� 4���� ��� �������� �׽�Ʈ
	b[0] = pFrustum->IsIn( (D3DXVECTOR3*)(pHeightMap+m_nCorner[0]) );
	b[1] = pFrustum->IsIn( (D3DXVECTOR3*)(pHeightMap+m_nCorner[1]) );
	b[2] = pFrustum->IsIn( (D3DXVECTOR3*)(pHeightMap+m_nCorner[2]) );
	b[3] = pFrustum->IsIn( (D3DXVECTOR3*)(pHeightMap+m_nCorner[3]) );

	// 4����� �������� �ȿ� ����
	if( (b[0] + b[1] + b[2] + b[3]) == 4 ) return FRUSTUM_COMPLETELY_IN;

	// �Ϻκ��� �������ҿ� �ִ� ���
	return FRUSTUM_PARTIALLY_IN;
}

// ��� �ڽĳ���� m_bCulled���� FALSE�� �Ѵ�
void	ZQuadTree::_AllInFrustum()
{
	m_bCulled = FALSE;
	if( !m_pChild[0] ) return;
	m_pChild[0]->_AllInFrustum();
	m_pChild[1]->_AllInFrustum();
	m_pChild[2]->_AllInFrustum();
	m_pChild[3]->_AllInFrustum();
}
// _IsInFrustum()�Լ��� ����� ���� �������� �ø� ����
void	ZQuadTree::_FrustumCull( TERRAINVERTEX* pHeightMap, ZFrustum* pFrustum )
{
	int ret;

	m_bCulled = FALSE;
	ret = _IsInFrustum( pHeightMap, pFrustum );
	switch( ret )
	{
		case FRUSTUM_COMPLETELY_IN :	// �������ҿ� ��������, ������� �˻� �ʿ����
			_AllInFrustum();
			return;
		case FRUSTUM_PARTIALLY_IN :		// �������ҿ� �Ϻ�����, ������� �˻� �ʿ���
			m_bCulled = FALSE;
			break;
		case FRUSTUM_OUT :				// �������ҿ��� �������, ������� �˻� �ʿ����
			m_bCulled = TRUE;
			return;
	}
	if( m_pChild[0] ) m_pChild[0]->_FrustumCull( pHeightMap, pFrustum );
	if( m_pChild[1] ) m_pChild[1]->_FrustumCull( pHeightMap, pFrustum );
	if( m_pChild[2] ) m_pChild[2]->_FrustumCull( pHeightMap, pFrustum );
	if( m_pChild[3] ) m_pChild[3]->_FrustumCull( pHeightMap, pFrustum );
}

// 4�� ����(���,�ϴ�,����,����)�� �̿���� �ε����� ���Ѵ�.
int	ZQuadTree::_GetNodeIndex( int ed, int cx, int& _0, int& _1, int& _2, int& _3 )
{
	int		n, _a, _b, _c, _d, gap;
	_a	= _0;
	_b	= _1;
	_c	= _2;
	_d  = _3;
	gap	= _b - _a;	// ���� ����� �¿�����

	switch( ed )
	{
		case EDGE_UP:	// ���� ���� �̿������ �ε���
			_0 = _a - cx * gap;
			_1 = _b - cx * gap;
			_2 = _a;
			_3 = _b;
			break;
		case EDGE_DN:	// �Ʒ� ���� �̿������ �ε���
			_0 = _c;
			_1 = _d;
			_2 = _c + cx * gap;
			_3 = _d + cx * gap;
			break;
		case EDGE_LT:	// ���� ���� �̿������ �ε���
			_0 = _a - gap;
			_1 = _a;
			_2 = _c - gap;
			_3 = _c;
			break;
		case EDGE_RT:	// ���� ���� �̿������ �ε���
			_0 = _b;
			_1 = _b + gap;
			_2 = _d;
			_3 = _d + gap;
			if( ( _0 / cx ) != ( _a / cx ) ) return -1;
			if( ( _1 / cx ) != ( _b / cx ) ) return -1;
			break;
	}

	n = ( _0 + _1 + _2 + _3 ) / 4;	// ��� �ε���
	if( !IS_IN_RANGE( n, 0, cx * cx - 1 ) ) return -1;

	return n;
}	

// ����Ʈ���� �˻��ؼ� 4�� �ڳʰ��� ��ġ�ϴ� ��带 ã�´�.
ZQuadTree* ZQuadTree::_FindNode( TERRAINVERTEX* pHeightMap, int _0, int _1, int _2, int _3 )
{
	ZQuadTree*	p = NULL;
	// ��ġ�ϴ� ����� ��尪�� ����
	if( (m_nCorner[0] == _0) && (m_nCorner[1] == _1) && (m_nCorner[2] == _2) && (m_nCorner[3] == _3) )
		return this;

	// �ڽ� ��尡 �ְ�?
	if( m_pChild[0] )
	{
		RECT	rc;
		POINT	pt;
		int n = ( _0 + _1 + _2 + _3 ) / 4;

		// ���� �ʻ󿡼��� ��ġ
		pt.x = (int)pHeightMap[n].p.x;
		pt.y = (int)pHeightMap[n].p.z;

		// 4���� �ڳʰ��� �������� �ڽĳ���� �� ���������� ��´�.
		SetRect( &rc, (int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].p.x, 
					  (int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_TL]].p.z, 
					  (int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].p.x, 
					  (int)pHeightMap[m_pChild[0]->m_nCorner[CORNER_BR]].p.z );
		// pt���� ���������ȿ� �ִٸ� �ڽĳ��� ����.
		if( IsInRect( &rc, pt ) )
			return m_pChild[0]->_FindNode( pHeightMap, _0, _1, _2, _3 );

		SetRect( &rc, (int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_TL]].p.x, 
					  (int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_TL]].p.z, 
					  (int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_BR]].p.x, 
					  (int)pHeightMap[m_pChild[1]->m_nCorner[CORNER_BR]].p.z );
		if( IsInRect( &rc, pt ) )
			return m_pChild[1]->_FindNode( pHeightMap, _0, _1, _2, _3 );

		SetRect( &rc, (int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_TL]].p.x, 
					  (int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_TL]].p.z, 
					  (int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_BR]].p.x, 
					  (int)pHeightMap[m_pChild[2]->m_nCorner[CORNER_BR]].p.z );
		if( IsInRect( &rc, pt ) )
			return m_pChild[2]->_FindNode( pHeightMap, _0, _1, _2, _3 );

		SetRect( &rc, (int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_TL]].p.x, 
					  (int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_TL]].p.z, 
					  (int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_BR]].p.x, 
					  (int)pHeightMap[m_pChild[3]->m_nCorner[CORNER_BR]].p.z );
		if( IsInRect( &rc, pt ) )
			return m_pChild[3]->_FindNode( pHeightMap, _0, _1, _2, _3 );
	}

	return NULL;
}

// �̿���带 �����.(�ﰢ�� ������ ������)
void	ZQuadTree::_BuildNeighborNode( ZQuadTree* pRoot, TERRAINVERTEX* pHeightMap, int cx )
{
	int				n;
	int				_0, _1, _2, _3;

	for( int i=0 ; i<4 ; i++ )
	{
		_0 = m_nCorner[0];
		_1 = m_nCorner[1];
		_2 = m_nCorner[2];
		_3 = m_nCorner[3];
		// �̿������ 4�� �ڳʰ��� ��´�.
		n = _GetNodeIndex( i, cx, _0, _1, _2, _3 );
		// �ڳʰ����� �̿������ �����͸� ���´�.
		if( n >= 0 ) m_pNeighbor[i] = pRoot->_FindNode( pHeightMap, _0, _1, _2, _3 );
	}

	// �ڽĳ��� ���ȣ��
	if( m_pChild[0] )
	{
		m_pChild[0]->_BuildNeighborNode( pRoot, pHeightMap, cx );
		m_pChild[1]->_BuildNeighborNode( pRoot, pHeightMap, cx );
		m_pChild[2]->_BuildNeighborNode( pRoot, pHeightMap, cx );
		m_pChild[3]->_BuildNeighborNode( pRoot, pHeightMap, cx );
	}
}

// ����Ʈ���� �����.(Build()�Լ����� �Ҹ���)
BOOL	ZQuadTree::_BuildQuadTree( TERRAINVERTEX* pHeightMap )
{
	if( _SubDivide() )
	{
		// ������ܰ�, ���� �ϴ��� �Ÿ��� ���Ѵ�.
		D3DXVECTOR3 v = *((D3DXVECTOR3*)(pHeightMap+m_nCorner[CORNER_TL])) - 
					    *((D3DXVECTOR3*)(pHeightMap+m_nCorner[CORNER_BR]));
		// v�� �Ÿ����� �� ��带 ���δ� ��豸�� �����̹Ƿ�, 
		// 2�� ������ �������� ���Ѵ�.
		m_fRadius	  = D3DXVec3Length( &v ) / 2.0f;
		m_pChild[CORNER_TL]->_BuildQuadTree( pHeightMap );
		m_pChild[CORNER_TR]->_BuildQuadTree( pHeightMap );
		m_pChild[CORNER_BL]->_BuildQuadTree( pHeightMap );
		m_pChild[CORNER_BR]->_BuildQuadTree( pHeightMap );
	}
	return TRUE;
}


// QuadTree�� �����Ѵ�.
BOOL	ZQuadTree::Build( TERRAINVERTEX* pHeightMap )
{
	// ����Ʈ�� ����
	_BuildQuadTree( pHeightMap );
	// �̿���� ����
	_BuildNeighborNode( this, pHeightMap, m_nCorner[CORNER_TR]+1 );
	return TRUE;
}

//	�ﰢ���� �ε����� �����, ����� �ﰢ���� ������ ��ȯ�Ѵ�.
int		ZQuadTree::GenerateIndex( LPVOID pIndex, TERRAINVERTEX* pHeightMap, ZFrustum* pFrustum, float fLODRatio )
{
	// ���� �������� �ø��� �ؼ� �ø��� ������ �����Ѵ�.
	_FrustumCull( pHeightMap, pFrustum );
	// ����� �������� �ε����� �����ѵ�, �������� ������ �����Ѵ�.
	return _GenTriIndex( 0, pIndex, pHeightMap, pFrustum, fLODRatio );
}

