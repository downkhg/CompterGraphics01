/**----------------------------------------------------------------------------
* \brief 광원 설정
* 파일: Lights.cpp
*
* 설명: 조명을 사용하면 훨씬더 극적인 연출이 가능하다.
*       조명을 사용하기 위해서는 광원이나 재질을 생성해야한다. 또한 기하정보에
*       노멀벡터정보를 포함하고 있어야 한다.
*       광원은 위치,색깔,방향등의 정보를 바탕으로 생성된다.
*------------------------------------------------------------------------------
*/
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>




/**-----------------------------------------------------------------------------
*  전역변수
*------------------------------------------------------------------------------
*/
LPDIRECT3D9             g_pD3D = NULL; /// D3D 디바이스를 생성할 D3D객체변수
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; /// 렌더링에 사용될 D3D디바이스
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; /// 정점을 보관할 정점버퍼

/// 사용자 정점을 정의할 구조체
/// 광원을 사용하기때문에 노멀벡터가 있어야 한다는 사실을 명심하자.
struct CUSTOMVERTEX
{
	D3DXVECTOR3 position; /// 정점의 3차원 좌표
	D3DXVECTOR3 normal;   /// 정점의 노멀 벡터
};

/// 사용자 정점 구조체에 관한 정보를 나타내는 FVF값
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)




/**-----------------------------------------------------------------------------
* Direct3D 초기화
*------------------------------------------------------------------------------
*/
HRESULT InitD3D(HWND hWnd)
{
	/// 디바이스를 생성하기위한 D3D객체 생성
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	/// 디바이스를 생성할 구조체
	/// 복잡한 오브젝트를 그릴것이기때문에, 이번에는 Z버퍼가 필요하다.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

	/// 디바이스 생성
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}


	//g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW); //컬링을켠다.
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //컬링을끈다.

	//렌더큐를 이용하여 가장멀리는 물체를 먼저 랜더링한다.
	//g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE); // Z버퍼기능을 켠다.
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE); // Z버퍼기능을 끈다.

	return S_OK;
}




/**-----------------------------------------------------------------------------
* 기하정보 초기화
*------------------------------------------------------------------------------
*/
HRESULT InitGeometry()
{
	//빛이 반사되어 색상이 정해지는 것이기때문에 색상에대한 설정이 없음.
	/// 정점버퍼 생성
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	/// 알고리즘을 사용해서 실린더(위 아래가 터진 원통)를 만든다.
	CUSTOMVERTEX* pVertices;
	if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return E_FAIL;
	for (DWORD i = 0; i < 50; i++)
	{
		FLOAT theta = (2 * D3DX_PI * i) / (50 - 1);
		pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));	/// 실린더의 아래쪽 원통의 좌표
		pVertices[2 * i + 0].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));	/// 실린더의 아래쪽 원통의 노멀
		pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));	/// 실린더의 위쪽 원통의 좌표
		pVertices[2 * i + 1].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));	/// 실린더의 위쪽 원통의 노멀
	}
	g_pVB->Unlock();

	return S_OK;
}




/**-----------------------------------------------------------------------------
* 초기화된 객체들 소거
*------------------------------------------------------------------------------
*/
VOID Cleanup()
{
	if (g_pVB != NULL)
		g_pVB->Release();

	if (g_pd3dDevice != NULL)
		g_pd3dDevice->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();
}



/**-----------------------------------------------------------------------------
* 행렬 설정
*------------------------------------------------------------------------------
*/
VOID SetupMatrices()
{
	/// 월드행렬
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);							/// 월드행렬을 단위행렬으로 설정
	D3DXMatrixRotationX(&matWorld, timeGetTime() / 500.0f);		/// X축을 중심으로 회전행렬 생성
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);		/// 디바이스에 월드행렬 설정

	/// 뷰행렬을 설정
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	/// 프로젝션 행렬 설정
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
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
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	//주변광: 주변에 물체에 비춰서 나타는 빛
	mtrl.Ambient.r = 0.0f;
	mtrl.Ambient.g = 0.0f;
	mtrl.Ambient.b = 1.0f;
	mtrl.Ambient.a = 1.0f;
	//확산광: 물체에 빛이 반사되어 나타는 빛(색상)
	mtrl.Diffuse.r = 1.0f;
	mtrl.Diffuse.g = 0.0f;
	mtrl.Diffuse.b = 0.0f;
	mtrl.Diffuse.a = 1.0f;
	g_pd3dDevice->SetMaterial(&mtrl);

	/// 광원 설정
	D3DXVECTOR3 vecDir;									/// 방향성 광원(directional light)이 향할 빛의 방향
	D3DLIGHT9 light;									/// 광원 구조체
	ZeroMemory(&light, sizeof(D3DLIGHT9));			/// 구조체를 0으로 지운다.
	light.Type = D3DLIGHT_DIRECTIONAL;			/// 광원의 종류(점 광원,방향성 광원,스포트라이트)
	light.Diffuse.r = 1.0f;							/// 광원의 색깔과 밝기
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	/*vecDir = vecDir(cosf(timeGetTime() / 350.0f),	/// 광원의 방향
		1.0f,
		sinf(timeGetTime() / 350.0f));*/
	vecDir = D3DXVECTOR3(0, 0, 1);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);	/// 광원의 방향을 단위벡터로 만든다.

	light.Range = 1000.0f;									/// 광원이 다다를수 있는 최대거리
	g_pd3dDevice->SetLight(0, &light);							/// 디바이스에 0번 광원 설치
	g_pd3dDevice->LightEnable(0, TRUE);							/// 0번 광원을 켠다
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			/// 광원설정을 켠다

	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);		/// 환경광원(ambient light)의 값 설정
}




/**-----------------------------------------------------------------------------
* 화면 그리기
*------------------------------------------------------------------------------
*/
VOID Render()
{
	/// 후면버퍼와 Z버퍼를 지운다.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	/// 렌더링 시작
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		/// 광원과 재질 설정
		SetupLights();

		/// 월드,뷰,프로젝션 행렬을 설정한다.
		SetupMatrices();

		/// 정점버퍼의 내용을 그린다.
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2);

		/// 렌더링 종료
		g_pd3dDevice->EndScene();
	}

	/// 후면버퍼를 보이는 화면으로!
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}




/**-----------------------------------------------------------------------------
* 윈도우 프로시져
*------------------------------------------------------------------------------
*/
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}




/**-----------------------------------------------------------------------------
* 프로그램 시작점
*------------------------------------------------------------------------------
*/
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
	/// 윈도우 클래스 등록
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
		GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
		"D3D Tutorial", NULL };
	RegisterClassEx(&wc);

	/// 윈도우 생성
	HWND hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial 04: Lights",
		WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);

	/// Direct3D 초기화
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		/// 기하정보 초기화
		if (SUCCEEDED(InitGeometry()))
		{
			/// 윈도우 출력
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			/// 메시지 루프
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					Render();
			}
		}
	}

	UnregisterClass("D3D Tutorial", wc.hInstance);
	return 0;
}



