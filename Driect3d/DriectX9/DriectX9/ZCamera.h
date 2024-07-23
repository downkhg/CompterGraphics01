#ifndef _ZCAMERA_H_
#define _ZCAMERA_H_

#include <d3d9.h>
#include <d3dx9.h>

/**
 * @brief ������ ī�޶� ���� Ŭ����
 * @author ������(shiva_72@hotmail.com)
 * @todo ���90���� �Ϲ�90���� üũ���� �ʾƼ�, ��ġ������ �Ҿ����ϴ�.
 */
class ZCamera
{
	D3DXVECTOR3		m_vEye;			/// ī�޶��� ���� ��ġ
	D3DXVECTOR3		m_vLookat;		/// ī�޶��� �ü� ��ġ
	D3DXVECTOR3		m_vUp;			/// ī�޶��� ��溤��

	D3DXVECTOR3		m_vView;		/// ī�޶� ���ϴ� �������⺤��
	D3DXVECTOR3		m_vCross;		/// ī������ ���麤�� cross( view, up )

	D3DXMATRIXA16	m_matView;		/// ī�޶� ���
	D3DXMATRIXA16	m_matBill;		/// ������ ���(ī�޶��� �����)
public:

	/// ������
	ZCamera();
	
	/// ī�޶� ����� ����.
	D3DXMATRIXA16*	GetViewMatrix() { return &m_matView; }
	
	/// ������ ����� ����.
	D3DXMATRIXA16*	GetBillMatrix() { return &m_matBill; }

public:

	/// ī�޶� ����� �����ϱ����� �⺻ ���Ͱ����� �����Ѵ�.
	D3DXMATRIXA16*	SetView( D3DXVECTOR3* pvEye,D3DXVECTOR3* pvLookat,D3DXVECTOR3* pvUp);

	/// ī�޶��� ��ġ���� �����Ѵ�.
	void			SetEye( D3DXVECTOR3* pv ) { m_vEye = *pv; }
	
	/// ī�޶��� ��ġ���� ����.
	D3DXVECTOR3*	GetEye() { return &m_vEye; }
	
	/// ī�޶��� �ü����� �����Ѵ�.
	void			SetLookat( D3DXVECTOR3* pv ) { m_vLookat = *pv; }

	/// ī�޶��� �ü����� ����.
	D3DXVECTOR3*	GetLookat() { return &m_vLookat; }
	
	/// ī�޶��� ��溤�Ͱ��� �����Ѵ�.
	void			SetUp( D3DXVECTOR3* pv ) { m_vUp = *pv; }
	
	/// ī�޶��� ��溤�Ͱ��� ����.
	D3DXVECTOR3*	GetUp() { return &m_vUp; }
	
	/// ���� �����Ѵ�.
	void			Flush() { SetView( &m_vEye, &m_vLookat, &m_vUp ); }

	/// ī�޶� ��ǥ���� X������ angle��ŭ ȸ���Ѵ�.
	D3DXMATRIXA16*	RotateLocalX( float angle );
	
	/// ī�޶� ��ǥ���� Y������ angle��ŭ ȸ���Ѵ�.
	D3DXMATRIXA16*	RotateLocalY( float angle );

//	D3DXMATRIXA16*	RotateLocalZ( float angle );

	/// ������ǥ���� *pv���� ��ġ�� �̵��Ѵ�.
	D3DXMATRIXA16*	MoveTo( D3DXVECTOR3* pv );
	
	/// ī�޶� ��ǥ���� X��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalX( float dist );

	/// ī�޶� ��ǥ���� Y��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalY( float dist );

	/// ī�޶� ��ǥ���� Z��������� dist��ŭ �����Ѵ�.(������ -dist�� ������ �ȴ�.)
	D3DXMATRIXA16*	MoveLocalZ( float dist );
};

#endif // _ZCAMERA_H_