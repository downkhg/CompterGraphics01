#include <d3d9.h>
#include <d3dx9.h>
#include "ZCamera.h"

// ������
ZCamera::ZCamera()
{
	D3DXVECTOR3	eye(0.0f,0.0f,0.0f);
	D3DXVECTOR3	lookat(0.0f,0.0f,-1.0f);
	D3DXVECTOR3	up(0.0f,1.0f,0.0f);
	D3DXMatrixIdentity( &m_matView );
	D3DXMatrixIdentity( &m_matBill );
	SetView( &eye, &lookat, &up );
}

/// ī�޶� ����� �����ϱ����� �⺻ ���Ͱ����� �����Ѵ�.
D3DXMATRIXA16*	ZCamera::SetView( D3DXVECTOR3* pvEye,D3DXVECTOR3* pvLookat,D3DXVECTOR3* pvUp)
{
	m_vEye		= *pvEye;
	m_vLookat	= *pvLookat;
	m_vUp		= *pvUp;
	D3DXVec3Normalize( &m_vView, &( m_vLookat - m_vEye ) );
	D3DXVec3Cross( &m_vCross, &m_vUp, &m_vView );

	D3DXMatrixLookAtLH( &m_matView, &m_vEye, &m_vLookat, &m_vUp);
	D3DXMatrixInverse( &m_matBill, NULL, &m_matView );
	m_matBill._41 = 0.0f;
	m_matBill._42 = 0.0f;
	m_matBill._43 = 0.0f;

	return &m_matView;
}

/// ī�޶� ��ǥ���� X������ angle��ŭ ȸ���Ѵ�.
D3DXMATRIXA16* ZCamera::RotateLocalX( float angle )
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis( &matRot, &m_vCross, angle );

	D3DXVECTOR3 vNewDst,vNewUp;
	D3DXVec3TransformCoord( &vNewDst, &m_vView, &matRot );	// view * rot�� ���ο� dst vector�� ���Ѵ�.
//	D3DXVec3Cross( &vNewUp, &vNewDst, &m_vCross );			// cross( dst, x��)���� up vector�� ���Ѵ�.
//	D3DXVec3Normalize( &vNewUp, &vNewUp );					// up vector�� unit vector��...
	vNewDst += m_vEye;										// ���� dst position =  eye Position + dst vector

	return SetView( &m_vEye, &vNewDst, &m_vUp );
}

/// ī�޶� ��ǥ���� Y������ angle��ŭ ȸ���Ѵ�.
D3DXMATRIXA16* ZCamera::RotateLocalY( float angle )
{
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationAxis( &matRot, &m_vUp, angle );

	D3DXVECTOR3 vNewDst;
	D3DXVec3TransformCoord( &vNewDst, &m_vView, &matRot );	// view * rot�� ���ο� dst vector�� ���Ѵ�.
	vNewDst += m_vEye;										// ���� dst position =  eye Position + dst vector

	return SetView( &m_vEye, &vNewDst, &m_vUp );
}

/// ī�޶� ��ǥ���� X��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
D3DXMATRIXA16* ZCamera::MoveLocalX( float dist )
{
	D3DXVECTOR3 vNewEye	= m_vEye;
	D3DXVECTOR3 vNewDst	= m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize( &vMove, &m_vCross );
	vMove	*= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView( &vNewEye, &vNewDst, &m_vUp );
}

/// ī�޶� ��ǥ���� Y��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
D3DXMATRIXA16* ZCamera::MoveLocalY( float dist )
{
	D3DXVECTOR3 vNewEye	= m_vEye;
	D3DXVECTOR3 vNewDst	= m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize( &vMove, &m_vUp );
	vMove	*= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView( &vNewEye, &vNewDst, &m_vUp );
}

/// ī�޶� ��ǥ���� Z��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
D3DXMATRIXA16* ZCamera::MoveLocalZ( float dist )
{
	D3DXVECTOR3 vNewEye	= m_vEye;
	D3DXVECTOR3 vNewDst	= m_vLookat;

	D3DXVECTOR3 vMove;
	D3DXVec3Normalize( &vMove, &m_vView );
	vMove	*= dist;
	vNewEye += vMove;
	vNewDst += vMove;

	return SetView( &vNewEye, &vNewDst, &m_vUp );
}

/// ������ǥ���� *pv���� ��ġ�� ī�޶� �̵��Ѵ�.
D3DXMATRIXA16* ZCamera::MoveTo( D3DXVECTOR3* pv )
{
	D3DXVECTOR3	dv = *pv - m_vEye;
	m_vEye = *pv;
	m_vLookat += dv;
	return SetView( &m_vEye, &m_vLookat, &m_vUp );
}
