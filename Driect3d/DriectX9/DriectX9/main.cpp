/**-----------------------------------------------------------------------------
* \brief 정점버퍼 생성
* 파일: Vertices.cpp
*
* 설명: 이번에는 정점을 렌더링하는 것을 공부하게 된다. 여기서 우리는 정점버퍼
*       (Vertex Buffer)개념을 보게될것이다. 정점버퍼란 정점을 보관하기 위한
*       D3D용 객체인데, 사용자가 FVF(Flexible Vertex Format)를 사용하여 자유롭
*       게 형식을 정의할 수 있다. 이번 학습에서는 변환과 광원처리가 완료된 정점
*       을 이용할 것이다.
*------------------------------------------------------------------------------
*/
#include <d3d9.h>




/**-----------------------------------------------------------------------------
*  전역변수
*------------------------------------------------------------------------------
*/
LPDIRECT3D9             g_pD3D = NULL; /// D3D 디바이스를 생성할 D3D객체변수
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; /// 렌더링에 사용될 D3D디바이스
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; /// 정점을 보관할 정점버퍼

									  /// 사용자 정점을 정의할 구조체
struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw; /// 정점의 변환된 좌표(rhw값이 있으면 변환이 완료된 정점이다.
	DWORD color;        /// 정점의 색깔
};

/// 사용자 정점 구조체에 관한 정보를 나타내는 FVF값
/// 구조체는 X,Y,Z,RHW값과 Diffuse색깔값으로 이루어져 있음을 알 수 있다.
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)




/**-----------------------------------------------------------------------------
* Direct3D 초기화
*------------------------------------------------------------------------------
*/
HRESULT InitD3D(HWND hWnd)
{
	/// 디바이스를 생성하기위한 D3D객체 생성
	//GPU는 CPU와 프로세스가 다르기때문에, GPU로부터 접근하기위해서 주소값을 포인터로 가져온다.
	if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
		return E_FAIL;

	/// 디바이스를 생성할 구조체
	//디바이스의 설정을 초기화 한다.
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	/// 디바이스 생성: 초기화된 값으로 3d디바이스 구조체를 초기화한다.
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &g_pd3dDevice)))
	{
		return E_FAIL;
	}

	/// 디바이스 상태정보를 처리할경우 여기에서 한다.

	return S_OK;
}




/**-----------------------------------------------------------------------------
* 정점버퍼를 생성하고 정점값을 채워넣는다.
* 정점버퍼란 기본적으로 정점정보를 갖고있는 메모리블럭이다.
* 정점버퍼를 생성한 다음에는 반드시 Lock()과 Unlock()으로 포인터를 얻어내서
* 정점정보를 정점버퍼에 써넣어야 한다.
* 또한 D3D는 인덱스버퍼도 사용가능하다는 것을 명심하자.
* 정점버퍼나 인덱스버퍼는 기본 시스템 메모리외에 디바이스 메모리(비디오카드 메모리)
* 에 생성될수 있는데, 대부분의 비디오카드에서는 이렇게 할경우 엄청난 속도의 향상을
* 얻을 수 있다.
*------------------------------------------------------------------------------
*/
HRESULT InitVB()
{
	/// 삼각형을 렌더링하기위해 세개의 정점을 선언
	CUSTOMVERTEX vertices[] = // x, y, z, rhw, color
	{
		{ 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, },  //1
		{ 50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, }, //3
		{ 250.0f, 250.0f, 0.5f, 1.0f, 0xffffffff, }, //2
	};

	/// 정점버퍼 생성
	/// 3개의 사용자정점을 보관할 메모리를 할당한다.
	/// FVF를 지정하여 보관할 데이터의 형식을 지정한다.
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	/// 정점버퍼를 값으로 채운다. 
	/// 정점버퍼의 Lock()함수를 호출하여 포인터를 얻어온다.
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, vertices, sizeof(vertices));
	g_pVB->Unlock();

	return S_OK;
}




/**-----------------------------------------------------------------------------
* 초기화 객체들 소거
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
* 화면 그리기
*------------------------------------------------------------------------------
*/
VOID Render()
{
	/// 후면버퍼를 파란색(0,0,255)으로 지운다.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 0, 255), 1.0f, 0);

	/// 렌더링 시작
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		/// 정점버퍼의 삼각형을 그린다.
		/// 1. 정점정보가 담겨있는 정점버퍼를 출력 스트림으로 할당한다.
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		/// 2. D3D에게 정점쉐이더 정보를 지정한다. 대부분의 경우에는 FVF만 지정한다.
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		/// 3. 기하 정보를 출력하기 위한 DrawPrimitive()함수 호출
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

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

	/// 윈도우 생성: WInAPI를 이용하여 윈도우 창을 생성한다.
	HWND hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial 02: Vertices",
		WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
		GetDesktopWindow(), NULL, wc.hInstance, NULL);

	/// Direct3D 초기화: 3d디바이스(GPU)를 초기화한다.
	if (SUCCEEDED(InitD3D(hWnd)))
	{
		/// 정점버퍼 초기화: 버텍스버퍼에 값을 초기화하고 3d디바이스 장치에 버텍스버퍼를 생성/복제한다.
		if (SUCCEEDED(InitVB()))
		{
			/// 윈도우 출력
			ShowWindow(hWnd, SW_SHOWDEFAULT);
			UpdateWindow(hWnd);

			/// 메시지 루프: 윈도우가 작동하는 중에 메세지를 받기위한 루프
			MSG msg;
			ZeroMemory(&msg, sizeof(msg));
			while (msg.message != WM_QUIT)
			{
				/// 메시지큐에 메시지가 있으면 메시지 처리
				if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
					/// 처리할 메시지가 없으면 Render()함수 호출
					//랜더링: 백버퍼를 초기화하고 다음 프레임을 렌더링하고, 전면버퍼와 교환한다.
					Render();
			}
		}
	}

	/// 등록된 클래스 소거
	UnregisterClass("D3D Tutorial", wc.hInstance);
	return 0;
}
