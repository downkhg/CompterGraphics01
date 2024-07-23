/**-----------------------------------------------------------------------------
 * \brief 빌보드처리
 * 파일: main.cpp
 *
 * 설명: 빌보드 처리방법을 익혀보자
 *       
 *------------------------------------------------------------------------------
 */

#define MAINBODY

#include <d3d9.h>
#include <d3dx9.h>
#include "ZCamera.h"
#include "ZFLog.h"
#include "ZWater.h"

#include <vector>
using namespace std;

#define WINDOW_W		500
#define WINDOW_H		500
#define WINDOW_TITLE	"Billboard"

/**-----------------------------------------------------------------------------
 *  전역변수
 *------------------------------------------------------------------------------
 */
HWND					g_hwnd = NULL;

LPDIRECT3D9             g_pD3D       = NULL; // D3D 디바이스를 생성할 D3D객체변수
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // 렌더링에 사용될 D3D디바이스
LPDIRECT3DTEXTURE9		g_pTexBillboard[4] = { NULL, NULL, NULL, NULL };// 빌보드로 사용할 텍스처

D3DXMATRIXA16			g_matAni;
D3DXMATRIXA16			g_matWorld;
D3DXMATRIXA16			g_matView;
D3DXMATRIXA16			g_matProj;

DWORD					g_dwMouseX = 0;			// 마우스의 좌표
DWORD					g_dwMouseY = 0;			// 마우스의 좌표
BOOL					g_bBillboard = TRUE;	// 빌보드처리를 할것인가?
BOOL					g_bWireframe = FALSE;	// 와이어프레임으로 그릴것인가?

ZCamera*				g_pCamera = NULL;	// Camera 클래스
ZWater*					g_pWater = NULL;

/**-----------------------------------------------------------------------------
 * Direct3D 초기화
 *------------------------------------------------------------------------------
 */
HRESULT InitD3D( HWND hWnd )
{
    // 디바이스를 생성하기위한 D3D객체 생성
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // 디바이스를 생성할 구조체
    // 복잡한 오브젝트를 그릴것이기때문에, 이번에는 Z버퍼가 필요하다.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    /// 디바이스 생성
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // 기본컬링, CCW
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );

    // Z버퍼기능을 켠다.
    g_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

    return S_OK;
}

/**-----------------------------------------------------------------------------
 * 행렬 설정
 *------------------------------------------------------------------------------
 */
void InitMatrix()
{
	/// 월드 행렬 설정
	D3DXMatrixIdentity( &g_matWorld );
    g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_matWorld );

    /// 뷰 행렬을 설정
    D3DXVECTOR3 vEyePt( 0.0f, 5.0f, (float)-3.0f );
    D3DXVECTOR3 vLookatPt( 0.0f, 5.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXMatrixLookAtLH( &g_matView, &vEyePt, &vLookatPt, &vUpVec );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &g_matView );

    /// 실제 프로젝션 행렬
	D3DXMatrixPerspectiveFovLH( &g_matProj, D3DX_PI/4, 1.0f, 1.0f, 1000.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &g_matProj );

	/// 프러스텀 컬링용 프로젝션 행렬
    D3DXMatrixPerspectiveFovLH( &g_matProj, D3DX_PI/4, 1.0f, 1.0f, 200.0f );

	/// 카메라 초기화
	g_pCamera->SetView( &vEyePt, &vLookatPt, &vUpVec );

}

/**-----------------------------------------------------------------------------
 * 기하정보 초기화
 *------------------------------------------------------------------------------
 */
HRESULT InitGeometry()
{
	InitMatrix();
	// 빌보드로 사용할 텍스처 이미지
	D3DXCreateTextureFromFile( g_pd3dDevice, "tree01S.dds", &g_pTexBillboard[0] );
	D3DXCreateTextureFromFile( g_pd3dDevice, "tree02S.dds", &g_pTexBillboard[1] );
	D3DXCreateTextureFromFile( g_pd3dDevice, "tree35S.dds", &g_pTexBillboard[2] );
	D3DXCreateTextureFromFile( g_pd3dDevice, "ExplosionEffect.png", &g_pTexBillboard[3] );


	// 최초의 마우스 위치 보관
	POINT	pt;
	GetCursorPos( &pt );
	g_dwMouseX = pt.x;
	g_dwMouseY = pt.y;
	return S_OK;
}

HRESULT InitObjects()
{
	g_pLog = new ZFLog( ZF_LOG_TARGET_WINDOW );
	g_pCamera = new ZCamera;
	g_pWater = new ZWater;
	g_pWater->Create( g_pd3dDevice, 64, 64, 100 );

	return S_OK;
}

void DeleteObjects()
{
	/// 등록된 클래스 소거
	S_DEL( g_pWater );
	S_DEL( g_pLog );
	S_DEL( g_pCamera );
}

/**-----------------------------------------------------------------------------
 * 초기화 객체들 소거
 *------------------------------------------------------------------------------
 */
VOID Cleanup()
{
	for( int i = 0 ; i < 4 ; i++ ) S_REL( g_pTexBillboard[i] );
	S_REL( g_pd3dDevice );
	S_REL( g_pD3D );
}


/**-----------------------------------------------------------------------------
 * 광원 설정
 *------------------------------------------------------------------------------
 */
VOID SetupLights()
{
    /// 재질(material)설정
    /// 재질은 디바이스에 단 하나만 설정될 수 있다.
    D3DMATERIAL9 mtrl;
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
    mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
    mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
    mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
    g_pd3dDevice->SetMaterial( &mtrl );

    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );			/// 광원설정을 켠다

    g_pd3dDevice->SetRenderState( D3DRS_AMBIENT, 0xffffffff );		/// 환경광원(ambient light)의 값 설정
}

/**-----------------------------------------------------------------------------
 * Status정보 출력
 *------------------------------------------------------------------------------
 */
void LogStatus( void )
{
	//g_pLog->Log( "Wireframe:%d", g_bWireframe );
	//g_pLog->Log( "BillBoard:%d", g_bBillboard );
}


/**-----------------------------------------------------------------------------
 * FPS(Frame Per Second)출력
 *------------------------------------------------------------------------------
 */
void LogFPS(void)
{
	static DWORD	nTick = 0;
	static DWORD	nFPS = 0;

	/// 1초가 지났는가?
	if( GetTickCount() - nTick > 1000 )
	{
		nTick = GetTickCount();
		/// FPS값 출력
		//g_pLog->Log("FPS:%d", nFPS );

		nFPS = 0;
		LogStatus();	/// 상태정보를 여기서 출력(1초에 한번)
		return;
	}
	nFPS++;
}


/**-----------------------------------------------------------------------------
 * 마우스 입력 처리
 *------------------------------------------------------------------------------
 */
void ProcessMouse( void )
{
	POINT	pt;
	float	fDelta = 0.001f;	// 마우스의 민감도, 이 값이 커질수록 많이 움직인다.

	GetCursorPos( &pt );
	int dx = pt.x - g_dwMouseX;	// 마우스의 변화값
	int dy = pt.y - g_dwMouseY;	// 마우스의 변화값

	g_pCamera->RotateLocalX( dy * fDelta );	// 마우스의 Y축 회전값은 3D world의  X축 회전값
	g_pCamera->RotateLocalY( dx * fDelta );	// 마우스의 X축 회전값은 3D world의  Y축 회전값
	D3DXMATRIXA16*	pmatView = g_pCamera->GetViewMatrix();		// 카메라 행렬을 얻는다.
	g_pd3dDevice->SetTransform( D3DTS_VIEW, pmatView );			// 카메라 행렬 셋팅


	// 마우스를 윈도우의 중앙으로 초기화
//	SetCursor( NULL );	// 마우스를 나타나지 않게 않다.
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
 * 키보드 입력 처리
 *------------------------------------------------------------------------------
 */
void ProcessKey( void )
{
	if( GetAsyncKeyState( 'A' ) ) g_pCamera->MoveLocalZ( 0.5f );	// 카메라 전진!
	if( GetAsyncKeyState( 'Z' ) ) g_pCamera->MoveLocalZ( -0.5f );	// 카메라 후진!
}

/**-----------------------------------------------------------------------------
 * 입력 처리
 *------------------------------------------------------------------------------
 */
void ProcessInputs( void )
{
	//ProcessMouse();
	ProcessKey();
}

/**-----------------------------------------------------------------------------
 * 애니메이션 설정
 *------------------------------------------------------------------------------
 */
VOID Animate()
{
	D3DXMatrixIdentity( &g_matAni );

	SetupLights();
	ProcessInputs();

	LogFPS();
}

struct MYVERTEX
{
	enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
	float px, py, pz;
	float tu, tv;
};

struct TexCoords
{
	float u;
	float v;
	TexCoords(float u = 0, float v = 0)
	{
		this->u = u;
		this->v = v;
	}
	void Print()
	{
		printf("(%f,%f)\n", this->u, this->v);
	}
};

vector<vector<vector<TexCoords>>> arrTextureCoords;
int nSellWidth = 4;
int nSellHighit = 4;
void InitTextureCoords()
{
	FILE* pFile = fopen("texCoords.txt", "rt");
	fscanf(pFile, "%d %d\n", &nSellWidth, &nSellHighit);
	arrTextureCoords.resize(nSellHighit);
	for (int y = 0; y < nSellHighit; y++)
	{
		arrTextureCoords[y].resize(nSellWidth);
		for (int x = 0; x < nSellWidth; x++)
		{
			arrTextureCoords[y][x].resize(4);
			for (int i = 0; i < 4; i++)
			{
				fscanf(pFile, "%f %f ", &arrTextureCoords[y][x][i].u, &arrTextureCoords[y][x][i].v);
				printf("%f %f ", arrTextureCoords[y][x][i].u, arrTextureCoords[y][x][i].v);
			}
			printf("\n");
		}
	}
	fclose(pFile);
}



int x = 0;
int y = 0;
void DrawBillboard()
{
	// 알파채널을 사용해서 투명텍스처 효과를 낸다
	g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
	g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, TRUE );
	g_pd3dDevice->SetRenderState( D3DRS_ALPHAREF,        0x08 );
	g_pd3dDevice->SetRenderState( D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL );

	struct MYVERTEX
	{
		enum { FVF = D3DFVF_XYZ | D3DFVF_TEX1 };
		float px, py, pz;
		float tu, tv;
	};

	// 빌보드 정점
	/*MYVERTEX vtx[4] = 
	{ 
		{ -1,  0, 0, 0, 1 },//tl
		{ -1,  4, 0, 0, 0 },//bl
		{  1,  0, 0, 1, 1 },//tr
		{  1,  4, 0, 1, 0 } //br
	};*/
	//a b
	//c d
	MYVERTEX vtx[4] =
	{
		{ -1,  -1, 0, 0, 0.25 },
		{ -1,  1, 0, 0, 0 },
		{  1,  -1, 0, 0.25, 0.25 },
		{  1,  1, 0, 0.25, 0 }
	};

	if (x < 4)
	{
		for (int i = 0; i < 4; i++)
		{
			vtx[i].tu = arrTextureCoords[y][x][i].u;
			vtx[i].tv = arrTextureCoords[y][x][i].v;
		}
		x++;
	}
	else
	{
		x = 0;
		if (y < 3) y++;
		else y = 0;
	}
	
	g_pLog->Log("[%d][%d]TL(%f,%f),TR(%f,%f),BL(%f,%f),BR(%f,%f)", x, y, vtx[0].tu, vtx[0].tv, vtx[2].tu, vtx[2].tv, vtx[1].tu, vtx[1].tv, vtx[3].tu, vtx[3].tv);

	D3DXMATRIXA16	matBillboard;
	D3DXMatrixIdentity( &matBillboard );

	// 0번 텍스처에 빌보드 텍스처를 올린다
	g_pd3dDevice->SetTexture( 1, NULL );
	g_pd3dDevice->SetFVF( MYVERTEX::FVF );


	if( g_bBillboard )
	{
		// Y축 회전행렬은 _11, _13, _31, _33번 행렬에 회전값이 들어간다
		// 카메라의 Y축 회전행렬값을 읽어서 역행렬을 만들면 X,Z축이 고정된
		// Y축 회전 빌보드 행렬을 만들수 있다
		/*matBillboard._11 = g_pCamera->GetViewMatrix()->_11;
		matBillboard._13 = g_pCamera->GetViewMatrix()->_13;
		matBillboard._31 = g_pCamera->GetViewMatrix()->_31;
		matBillboard._33 = g_pCamera->GetViewMatrix()->_33;*/
		//회전축 동기화
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 3; x++)
			{
				matBillboard.m[y][x] = g_pCamera->GetBillMatrix()->m[y][x];
			}
		}

		D3DXMatrixInverse( &matBillboard, NULL, &matBillboard );
	}

	// 빌보드의 좌표를 바꿔가며 찍는다
	for( int z = 0 ; z <= 40 ; z += 5 )
	{
		for( int x = 0 ; x <= 40 ; x += 5 )
		{
			matBillboard._41 = x - 20;	
			matBillboard._42 = 0;
			matBillboard._43 = z - 20;
			g_pd3dDevice->SetTexture( 0, g_pTexBillboard[(x+z)%4] );
			g_pd3dDevice->SetTransform( D3DTS_WORLD, &matBillboard );
			g_pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLESTRIP, 2, vtx, sizeof(MYVERTEX) );
		}
	}

	g_pd3dDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE );
	g_pd3dDevice->SetTransform( D3DTS_WORLD, &g_matWorld );
}


/**-----------------------------------------------------------------------------
 * 화면 그리기
 *------------------------------------------------------------------------------
 */
VOID Render()
{
    /// 후면버퍼와 Z버퍼 초기화
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(200,200,200), 1.0f, 0 );
	g_pd3dDevice->SetRenderState( D3DRS_FILLMODE, g_bWireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID );

	/// 애니메이션 행렬설정
	Animate();
    /// 렌더링 시작
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
		//g_pWater->Draw();

		DrawBillboard();
		/// 렌더링 종료
		g_pd3dDevice->EndScene();
    }

    /// 후면버퍼를 보이는 화면으로!
	Sleep(500);
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}




/**-----------------------------------------------------------------------------
 * 윈도우 프로시져
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
					g_bBillboard = !g_bBillboard;
					break;
			}
			break;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




/**-----------------------------------------------------------------------------
 * 프로그램 시작점
 *------------------------------------------------------------------------------
 */
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
    /// 윈도우 클래스 등록
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "BasicFrame", NULL };
    RegisterClassEx( &wc );

    /// 윈도우 생성
    HWND hWnd = CreateWindow( "BasicFrame", WINDOW_TITLE,
                              WS_OVERLAPPEDWINDOW, 100, 100, WINDOW_W, WINDOW_H,
                              GetDesktopWindow(), NULL, wc.hInstance, NULL );

	g_hwnd = hWnd;

	srand( GetTickCount() );
	InitTextureCoords();
    /// Direct3D 초기화
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
		if( SUCCEEDED( InitObjects() ) )
		{
			if( SUCCEEDED( InitGeometry() ) )
			{

        		/// 윈도우 출력
				ShowWindow( hWnd, SW_SHOWDEFAULT );
				UpdateWindow( hWnd );

        		/// 메시지 루프
				MSG msg;
				ZeroMemory( &msg, sizeof(msg) );
				while( msg.message!=WM_QUIT )
				{
            		/// 메시지큐에 메시지가 있으면 메시지 처리
					if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
					{
						TranslateMessage( &msg );
						DispatchMessage( &msg );
					}
					else
					/// 처리할 메시지가 없으면 Render()함수 호출
						Render();
				}
			}
		}
    }

	DeleteObjects();
    UnregisterClass( "BasicFrame", wc.hInstance );
    return 0;
}
