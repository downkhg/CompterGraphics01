/**----------------------------------------------------------------------------
 * \brief 메시 사용
 * 파일: Meshes.cpp
 *
 * 설명: 보다 멋진 기하정보를 출력하기 위해서는 전문적인 3D모델러가 만든 모델을
 *       파일로 읽어들이는 것이 일반적이다. 다행스럽게도 D3DX에는 강력한 X파일
 *       처리기능이 있어서 정점버퍼,인덱스버퍼 생성등의 많은 부분을 대신해준다.
 *       이번 예제는 D3DXMESH를 사용하여 파일을 읽어서 이 파일과 연관된 재질과
 *       텍스쳐를 함께 사용하는 것을 알아보자.
 *
 *       이번에 소개되지는 않지만 나중에 사용하게될 강력한 기능중에 하나가
 *       FVF를 지정하여 새로운 메시를 복제(clone)하는 것이다. 이 기능을 사용하여
 *       텍스쳐 좌표나 노멀벡터등을 기존메시에 추가한 새로운 메시를 생성할 수 있다.
 *       (나중에 Cg,HLSL등을 공부할때 많이 사용될 것이다.)
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

LPD3DXMESH              g_pMesh = NULL; // 메시 객체
D3DMATERIAL9* g_pMeshMaterials = NULL; // 메시에서 사용할 재질
LPDIRECT3DTEXTURE9* g_pMeshTextures = NULL; // 메시에서 사용할 텍스쳐
DWORD                   g_dwNumMaterials = 0L;   // 메시에서 사용중인 재질의 개수




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

    /// Z버퍼기능을 켠다.
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

    /// 주변광원값을 최대밝기로
    g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

    return S_OK;
}




/**-----------------------------------------------------------------------------
 * 기하정보 초기화
 * 메시읽기, 재질과 텍스쳐 배열 생성
 *------------------------------------------------------------------------------
 */
HRESULT InitGeometry()
{
    /// 재질을 임시로 보관할 버퍼선언
    LPD3DXBUFFER pD3DXMtrlBuffer;
    //x파일에 설정된 버텍스버퍼,인덱스버퍼,텍스처UV등의 정보가 포함된다. 
    /// Tiger.x파일을 메시로 읽어들인다. 이때 재질정보도 함께 읽는다.
    if (FAILED(D3DXLoadMeshFromX("Tiger.x", D3DXMESH_SYSTEMMEM,
        g_pd3dDevice, NULL,
        &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
        &g_pMesh)))
    {
        /// 현재 폴더에 파일이 없으면 상위폴더 검색
        if (FAILED(D3DXLoadMeshFromX("..\\Tiger.x", D3DXMESH_SYSTEMMEM,
            g_pd3dDevice, NULL,
            &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
            &g_pMesh)))
        {
            MessageBox(NULL, "Could not find tiger.x", "Meshes.exe", MB_OK);
            return E_FAIL;
        }
    }

    /// 재질정보와 텍스쳐 정보를 따로 뽑아낸다.
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];			/// 재질개수만큼 재질구조체 배열 생성
    g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];	/// 재질개수만큼 텍스쳐 배열 생성

    for (DWORD i = 0; i < g_dwNumMaterials; i++)
    {
        /// 재질정보를 복사
        g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        /// 주변광원정보를 Diffuse정보로
        g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;

        g_pMeshTextures[i] = NULL;
        if (d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
        {
            /// 텍스쳐를 파일에서 로드한다
            if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice,
                d3dxMaterials[i].pTextureFilename,
                &g_pMeshTextures[i])))
            {
                /// 텍스쳐가 현재 폴더에 없으면 상위폴더 검색
                const TCHAR* strPrefix = TEXT("..\\");
                const int lenPrefix = lstrlen(strPrefix);
                TCHAR strTexture[MAX_PATH];
                lstrcpyn(strTexture, strPrefix, MAX_PATH);
                lstrcpyn(strTexture + lenPrefix, d3dxMaterials[i].pTextureFilename, MAX_PATH - lenPrefix);
                if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice,
                    strTexture,
                    &g_pMeshTextures[i])))
                {
                    MessageBox(NULL, "Could not find texture map", "Meshes.exe", MB_OK);
                }
            }
        }
    }

    /// 임시로 생성한 재질버퍼 소거
    pD3DXMtrlBuffer->Release();

    return S_OK;
}




/**-----------------------------------------------------------------------------
 * 초기화된 객체들 소거
 *------------------------------------------------------------------------------
 */
VOID Cleanup()
{
    if (g_pMeshMaterials != NULL)
        delete[] g_pMeshMaterials;

    if (g_pMeshTextures)
    {
        for (DWORD i = 0; i < g_dwNumMaterials; i++)
        {
            if (g_pMeshTextures[i])
                g_pMeshTextures[i]->Release();
        }
        delete[] g_pMeshTextures;
    }
    if (g_pMesh != NULL)
        g_pMesh->Release();

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
    D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
    g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

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
 * 화면 그리기
 *------------------------------------------------------------------------------
 */
VOID Render()
{
    /// 후면버퍼와 Z버퍼를 지운다.
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

    /// 렌더링 시작
    if (SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        /// 월드,뷰,프로젝션 행렬을 설정한다.
        SetupMatrices();

        /// 메시는 재질이 다른 메시별로 부분집합을 이루고 있다.
        /// 이들을 루프를 수행해서 모두 그려준다.
        for (DWORD i = 0; i < g_dwNumMaterials; i++)
        {
            /// 부분집합 메시의 재질과 텍스쳐 설정
            g_pd3dDevice->SetMaterial(&g_pMeshMaterials[i]);
            g_pd3dDevice->SetTexture(0, g_pMeshTextures[i]);

            /// 부분집합 메시 출력
            g_pMesh->DrawSubset(i);
        }

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
    HWND hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial 06: Meshes",
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



