/**-----------------------------------------------------------------------------
 * \brief ���̸�
 * ����: HeightMap.cpp
 *
 * ����: QuadTree+LOD+Triangle Crack fix
 *       
 *------------------------------------------------------------------------------
 */

#define MAINBODY

#include <d3d9.h>
#include <d3dx9.h>
#include "ZCamera.h"
#include "ZFrustum.h"
#include "ZTerrain.h"
#include "ZFLog.h"

#define WINDOW_W		500
#define WINDOW_H		500
#define WINDOW_TITLE	"QuadTree+TriangleCrackFixed"
#define BMP_HEIGHTMAP	"map129.bmp"
/**-----------------------------------------------------------------------------
 *  ��������
 *------------------------------------------------------------------------------
 */
HWND					g_hwnd = NULL;

LPDIRECT3D9             g_pD3D       = NULL; // D3D ����̽��� ������ D3D��ü����
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // �������� ���� D3D����̽�

D3DXMATRIXA16			g_matAni;
D3DXMATRIXA16			g_matWorld;
D3DXMATRIXA16			g_matView;
D3DXMATRIXA16			g_matProj;

DWORD					g_dwMouseX = 0;			// ���콺�� ��ǥ
DWORD					g_dwMouseY = 0;			// ���콺�� ��ǥ
BOOL					g_bHideFrustum = TRUE;	// Frustum�� �ȱ׸� ���ΰ�?
BOOL					g_bLockFrustum = FALSE;	// Frustum�� ������ ���ΰ�?
BOOL					g_bWireframe = FALSE;	// ���̾����������� �׸����ΰ�?

//ZFLog*					g_pLog = NULL;		// Log�� ZFLog.h���� �ڵ����� ����ȴ�
ZCamera*				g_pCamera = NULL;	// Camera Ŭ����
ZFrustum*				g_pFrustum = NULL;	// Frustum Ŭ����
ZTerrain*				g_pTerrain = NULL;

/**-----------------------------------------------------------------------------
 * Direct3D �ʱ�ȭ
 *------------------------------------------------------------------------------
 */
HRESULT InitD3D( HWND hWnd )
{
    // ����̽��� �����ϱ����� D3D��ü ����
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // ����̽��� ������ ����ü
    // ������ ������Ʈ�� �׸����̱⶧����, �̹����� Z���۰� �ʿ��ϴ�.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    /// ����̽� ����
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // �⺻�ø�, CCW
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

    // Z���۱���� �Ҵ�.
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    return S_OK;
}

/**-----------------------------------------------------------------------------
 * ��� ����
 *------------------------------------------------------------------------------
 */
void InitMatrix()
{
	/// ���� ��� ����
	D3DXMatrixIdentity( &g_matWorld );
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_matWorld );

    /// �� ����� ����
    D3DXVECTOR3 vEyePt( 0.0f, 50.0f, (float)-30.0f );
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &g_matView, &vEyePt, &vLookatPt, &vUpVec );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &g_matView );

    /// ���� �������� ���
	D3DXMatrixPerspectiveFovLH( &g_matProj, D3DX_PI/4, 1.0f, 1.0f, 1000.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_matProj );

	/// �������� �ø��� �������� ���
    D3DXMatrixPerspectiveFovLH( &g_matProj, D3DX_PI/4, 1.0f, 1.0f, 200.0f );

	/// ī�޶� �ʱ�ȭ
	g_pCamera->SetView( &vEyePt, &vLookatPt, &vUpVec );

}

/**-----------------------------------------------------------------------------
 * �������� �ʱ�ȭ
 *------------------------------------------------------------------------------
 */
HRESULT InitGeometry()
{
	InitMatrix();

	// ������ ���콺 ��ġ ����
	POINT	pt;
	GetCursorPos( &pt );
	g_dwMouseX = pt.x;
	g_dwMouseY = pt.y;
	return S_OK;
}

HRESULT InitObjects()
{
	const char*	tex[4] = { 
		"tile2.tga", 
		"lightmap.tga", 
		"", 
		"" };
	D3DXVECTOR3	vScale;

	vScale.x = vScale.z = 1.0f; vScale.y = .1f;
//	g_pLog = new ZFLog( ZF_LOG_TARGET_WINDOW | ZF_LOG_TARGET_FILE, "log.txt" );
	g_pLog = new ZFLog( ZF_LOG_TARGET_WINDOW );
	g_pCamera = new ZCamera;
	g_pFrustum = new ZFrustum;
	g_pTerrain = new ZTerrain;
	if( FAILED( g_pTerrain->Create( g_pd3dDevice, &vScale, 0.05f, BMP_HEIGHTMAP, tex ) ) )
		return E_FAIL;

	return S_OK;
}

void DeleteObjects()
{
	/// ��ϵ� Ŭ���� �Ұ�
	DEL( g_pTerrain );
	DEL( g_pFrustum );
	DEL( g_pLog );
	DEL( g_pCamera );
}

/**-----------------------------------------------------------------------------
 * �ʱ�ȭ ��ü�� �Ұ�
 *------------------------------------------------------------------------------
 */
VOID Cleanup()
{
	REL( g_pd3dDevice );
	REL( g_pD3D );
}


/**-----------------------------------------------------------------------------
 * ���� ����
 *------------------------------------------------------------------------------
 */
VOID SetupLights()
{
    /// ����(material)����
    /// ������ ����̽��� �� �ϳ��� ������ �� �ִ�.
    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
    g_pd3dDevice->SetMaterial( &mtrl );

    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );			/// ���������� �Ҵ�

    g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );		/// ȯ�汤��(ambient light)�� �� ����
}

/**-----------------------------------------------------------------------------
 * Status���� ���
 *------------------------------------------------------------------------------
 */
void LogStatus( void )
{
	g_pLog->Log( "Wireframe:%d", g_bWireframe );
	g_pLog->Log( "HideFrustum:%d", g_bHideFrustum );
	g_pLog->Log( "LockFrustum:%d", g_bLockFrustum );
}


/**-----------------------------------------------------------------------------
 * FPS(Frame Per Second)���
 *------------------------------------------------------------------------------
 */
void LogFPS(void)
{
	static DWORD	nTick = 0;
	static DWORD	nFPS = 0;

	/// 1�ʰ� �����°�?
	if( GetTickCount() - nTick > 1000 )
	{
		nTick = GetTickCount();
		/// FPS�� ���
		g_pLog->Log("FPS:%d", nFPS );

		nFPS = 0;
		LogStatus();	/// ���������� ���⼭ ���(1�ʿ� �ѹ�)
		return;
	}
	nFPS++;
}


/**-----------------------------------------------------------------------------
 * ���콺 �Է� ó��
 *------------------------------------------------------------------------------
 */
void ProcessMouse( void )
{
	POINT	pt;
	float	fDelta = 0.001f;	// ���콺�� �ΰ���, �� ���� Ŀ������ ���� �����δ�.

	GetCursorPos( &pt );
	int dx = pt.x - g_dwMouseX;	// ���콺�� ��ȭ��
	int dy = pt.y - g_dwMouseY;	// ���콺�� ��ȭ��

	g_pCamera->RotateLocalX( dy * fDelta );	// ���콺�� Y�� ȸ������ 3D world��  X�� ȸ����
	g_pCamera->RotateLocalY( dx * fDelta );	// ���콺�� X�� ȸ������ 3D world��  Y�� ȸ����
	D3DXMATRIXA16*	pmatView = g_pCamera->GetViewMatrix();		// ī�޶� ����� ��´�.
	g_pd3dDevice->SetTransform( D3DTS_VIEW, pmatView );			// ī�޶� ��� ����


	// ���콺�� �������� �߾����� �ʱ�ȭ
//	SetCursor( NULL );	// ���콺�� ��Ÿ���� �ʰ� �ʴ�.
	RECT	rc;
	GetClientRect( g_hwnd, &rc );
	pt.x = (rc.right - rc.left) / 2;
	pt.y = (rc.bottom - rc.top) / 2;
	ClientToScreen( g_hwnd, &pt );
	SetCursorPos( pt.x, pt.y );
	g_dwMouseX = pt.x;
	g_dwMouseY = pt.y;
}

/**-----------------------------------------------------------------------------
 * Ű���� �Է� ó��
 *------------------------------------------------------------------------------
 */
void ProcessKey( void )
{
	if( GetAsyncKeyState( 'A' ) ) g_pCamera->MoveLocalZ( 0.5f );	// ī�޶� ����!
	if( GetAsyncKeyState( 'Z' ) ) g_pCamera->MoveLocalZ( -0.5f );	// ī�޶� ����!
}

/**-----------------------------------------------------------------------------
 * �Է� ó��
 *------------------------------------------------------------------------------
 */
void ProcessInputs( void )
{
	ProcessMouse();
	ProcessKey();
}

/**-----------------------------------------------------------------------------
 * �ִϸ��̼� ����
 *------------------------------------------------------------------------------
 */
VOID Animate()
{
	D3DXMatrixIdentity( &g_matAni );

	SetupLights();
	ProcessInputs();

	D3DXMATRIXA16	m;
	D3DXMATRIXA16	*pView;
	pView = g_pCamera->GetViewMatrix();	// ī�޶� Ŭ�����κ��� ��������� ��´�.
	m = *pView * g_matProj;				// World��ǥ�� ������ؼ� View * Proj����� ����Ѵ�.
	if( !g_bLockFrustum ) g_pFrustum->Make( &m );	// View*Proj��ķ� Frustum�� �����.

	LogFPS();
}


/**-----------------------------------------------------------------------------
 * ȭ�� �׸���
 *------------------------------------------------------------------------------
 */
VOID Render()
{
    /// �ĸ���ۿ� Z���� �ʱ�ȭ
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200,200,200), 1.0f, 0 );
	g_pd3dDevice->SetRenderState( D3DRS_FILLMODE, g_bWireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID );

	/// �ִϸ��̼� ��ļ���
	Animate();
    /// ������ ����
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
		g_pTerrain->Draw( g_pFrustum );
		if( !g_bHideFrustum ) g_pFrustum->Draw( g_pd3dDevice );

		/// ������ ����
		g_pd3dDevice->EndScene();
    }

    /// �ĸ���۸� ���̴� ȭ������!
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}




/**-----------------------------------------------------------------------------
 * ������ ���ν���
 *------------------------------------------------------------------------------
 */
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY :
            Cleanup();
            PostQuitMessage( 0 );
            return 0;
		case WM_KEYDOWN : 
			switch( wParam )
			{
				case VK_ESCAPE :
					PostMessage( hWnd, WM_DESTROY, 0, 0L );
					break;
				case '1' :
					g_bWireframe = !g_bWireframe;
					break;
				case '2' :
					g_bLockFrustum = !g_bLockFrustum;
					g_bHideFrustum = !g_bLockFrustum;
					break;
			}
			break;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




/**-----------------------------------------------------------------------------
 * ���α׷� ������
 *------------------------------------------------------------------------------
 */
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR , INT )
{
    /// ������ Ŭ���� ���
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "BasicFrame", NULL };
    RegisterClassEx( &wc );

    /// ������ ����
    HWND hWnd = CreateWindow( "BasicFrame", WINDOW_TITLE,
                              WS_OVERLAPPEDWINDOW, 100, 100, WINDOW_W, WINDOW_H,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

	g_hwnd = hWnd;

    /// Direct3D �ʱ�ȭ
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
		if( SUCCEEDED( InitObjects() ) )
		{
			if( SUCCEEDED( InitGeometry() ) )
			{

        		/// ������ ���
				ShowWindow( hWnd, SW_SHOWDEFAULT );
				UpdateWindow( hWnd );

        		/// �޽��� ����
				MSG msg;
				ZeroMemory( &msg, sizeof(msg) );
				while( msg.message!=WM_QUIT )
				{
            		/// �޽���ť�� �޽����� ������ �޽��� ó��
					if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
					{
						TranslateMessage( &msg );
						DispatchMessage( &msg );
					}
					else
					/// ó���� �޽����� ������ Render()�Լ� ȣ��
						Render();
				}
			}
		}
    }

	DeleteObjects();
    UnregisterClass( "D3D Tutorial", wc.hInstance );
    return 0;
}
