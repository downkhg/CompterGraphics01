/**----------------------------------------------------------------------------
 * \brief ��� ���
 * ����: Matrices.cpp
 *
 * ���� : ����̽��� ������ �����ϴ� ���� �˾Ҵ�.
 *        3���� ������ ��������� �ٷ�� ���ؼ��� 4x4ũ���� ����� ����ؾ� �Ѵ�.
 *        �⺻���� ��ĺ�ȯ���� �̵�(translations), ȸ��(rotations), ũ��(scaling)�� �ִ�.
 *       the geometry with translations, rotations, scaling, and setting up our
 *       camera.
 *
 *       ���������� ����ǥ�踦 ����ϴµ�, �̸� �켱 3���� ������ǥ��� ��ȯ�ؾ� �Ѵ�.
 *       �̶� ��������� ���ȴ�.
 *       �ٽ� ������ǥ���� ���������� ī�޶���ǥ��� ��ȯ�Ѵ�. �̶� ���Ǵ°��� ������̴�.
 *       �ٽ� �� ���������� 2�������� ���(viewport)�� �����ؾ� �����쿡 �׷����� �ִ�.
 *       ��, world -> view -> proejction�� ��ĺ�ȯ�� ��ģ�ڿ� ��μ� �׷����� �ִ� ���̴�.
 *       (����, ���� Ŭ���ε��� ó���� �߰������� �̷������.)
 *
 *       OpenGL������ ��Ŀ��� �Լ��� ���� �ۼ��ؾ� �ϰ�����, D3D���� D3DX��� ��ƿ��Ƽ
 *       �Լ����� �ټ� �����Ѵ�. ���⼭�� D3DX�迭 �Լ��� ����� ���̴�.
 **-----------------------------------------------------------------------------
 */
#include <Windows.h>
#include <mmsystem.h>	/// TimeGetTime()�Լ��� ����ϱ� ���ؼ� �����ϴ� ���
#include <d3dx9.h>




 /**-----------------------------------------------------------------------------
  *  ��������
  *------------------------------------------------------------------------------
  */
LPDIRECT3D9             g_pD3D = NULL; /// D3D ����̽��� ������ D3D��ü����
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; /// �������� ���� D3D����̽�
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; /// ������ ������ ��������

/// ����� ������ ������ ����ü
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      /// ������ 3���� ��ǥ
    DWORD color;        /// ������ ����
};

/// ����� ���� ����ü�� ���� ������ ��Ÿ���� FVF��
/// ����ü�� X,Y,Z���� Diffuse�������� �̷���� ������ �� �� �ִ�.
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)




/**-----------------------------------------------------------------------------
 * Direct3D �ʱ�ȭ
 *------------------------------------------------------------------------------
 */
HRESULT InitD3D(HWND hWnd)
{
    /// ����̽��� �����ϱ����� D3D��ü ����
    if (NULL == (g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
        return E_FAIL;

    /// ����̽��� ������ ����ü
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    /// ����̽� ����
    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &g_pd3dDevice)))
    {
        return E_FAIL;
    }

    /// �ø������ ����. �ﰢ���� �ո�, �޸��� ��� �������Ѵ�.
    g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    /// ������ ������ �����Ƿ�, ��������� ����.
    //���� ������ �����Ǿ����������Ƿ�, �����ɼ��� ���� ���� �����ֵ��� �����.
    //��������ȿ��� ���� ���̵��� �ùķ��̼ǵǱ� ����.
    g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

    return S_OK;
}




/**-----------------------------------------------------------------------------
 * �������� �ʱ�ȭ: Geometry �ﰢ���� �𿩼� ������� � ������ ��ü
 *------------------------------------------------------------------------------
 */
HRESULT InitGeometry()
{
    /// �ﰢ���� �������ϱ����� ������ ������ ����
    CUSTOMVERTEX g_Vertices[] =
    {
        { -1.0f,-1.0f, 0.0f, 0xffff0000, },
        {  1.0f,-1.0f, 0.0f, 0xff0000ff, },
        {  0.0f, 1.0f, 0.0f, 0xffffffff, },
    };

    /// �������� ����
    if (FAILED(g_pd3dDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
        0, D3DFVF_CUSTOMVERTEX,
        D3DPOOL_DEFAULT, &g_pVB, NULL)))
    {
        return E_FAIL;
    }

    /// �������۸� ������ ä���. 
    VOID* pVertices;
    if (FAILED(g_pVB->Lock(0, sizeof(g_Vertices), (void**)&pVertices, 0)))
        return E_FAIL;
    memcpy(pVertices, g_Vertices, sizeof(g_Vertices));
    g_pVB->Unlock();

    return S_OK;
}




/**-----------------------------------------------------------------------------
 * �ʱ�ȭ�� ��ü�� �Ұ�
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
 * ��� ����
 * ����� ������ �ְ�, ���� world, view, projection ����̴�.
 *------------------------------------------------------------------------------
 */
VOID SetupMatrices()
{
    /// �������
    D3DXMATRIXA16 matWorld;


    UINT  iTime = timeGetTime() % 1000;					/// float������ ���е��� ���ؼ� 1000���� ������ �����Ѵ�.
    FLOAT fAngle = iTime * (2.0f * D3DX_PI) / 1000.0f;		/// 1000�и��ʸ��� �ѹ�����(2 * pi) ȸ�� �ִϸ��̼� ����� �����.
    D3DXMatrixRotationY(&matWorld, fAngle);				/// Y���� ȸ����� ����
    g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);	/// ������ ȸ������� ������ķ� ����̽��� ����

    /// ������� �����ϱ� ���ؼ��� ���������� �ʿ��ϴ�.    
    D3DXVECTOR3 vEyePt(0.0f, 0.0f, -5.0f);							/// 1. ���� ��ġ( 0, 3.0, -5)
    D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);						/// 2. ���� �ٶ󺸴� ��ġ( 0, 0, 0 )
    D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);							/// 3. õ�������� ��Ÿ���� ��溤��( 0, 1, 0 )
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);	/// 1,2,3�� ������ ����� ����
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);				/// ������ ������� ����̽��� ����

    /// �������� ����� �����ϱ� ���ؼ��� �þ߰�(FOV=Field Of View)�� ��Ⱦ��(aspect ratio), Ŭ���� ����� ���� �ʿ��ϴ�.
    D3DXMATRIXA16 matProj;
    /// matProj   : ���� ������ ���
    /// D3DX_PI/4 : FOV(D3DX_PI/4 = 45��)
    /// 1.0f      : ��Ⱦ��
    /// 1.0f      : ���� Ŭ���� ���(near clipping plane)
    /// 100.0f    : ���Ÿ� Ŭ���� ���(far clipping plane)
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
    g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);		/// ������ �������� ����� ����̽��� ����
}



/**-----------------------------------------------------------------------------
 * ȭ�� �׸���
 *------------------------------------------------------------------------------
 */
VOID Render()
{
    /// �ĸ���۸� �Ķ���(0,0,255)���� �����.
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

    /// ������ ����
    if (SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        /// ����,��,�������� ����� �����Ѵ�.
        SetupMatrices();

        /// ���������� ������ �׸���.
        g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
        g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
        g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 1);

        /// ������ ����
        g_pd3dDevice->EndScene();
    }

    /// �ĸ���۸� ���̴� ȭ������!
    g_pd3dDevice->Present(NULL, NULL, NULL, NULL);
}




/**-----------------------------------------------------------------------------
 * ������ ���ν���
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
 * ���α׷� ������
 *------------------------------------------------------------------------------
 */
INT WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, INT)
{
    /// ������ Ŭ���� ���
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      "D3D Tutorial", NULL };
    RegisterClassEx(&wc);

    /// ������ ����
    HWND hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial 03: Matrices",
        WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
        GetDesktopWindow(), NULL, wc.hInstance, NULL);

    /// Direct3D �ʱ�ȭ
    if (SUCCEEDED(InitD3D(hWnd)))
    {
        /// ��鿡 ���� �������� �ʱ�ȭ
        if (SUCCEEDED(InitGeometry()))
        {
            /// ������ ���
            ShowWindow(hWnd, SW_SHOWDEFAULT);
            UpdateWindow(hWnd);

            /// �޽��� ����
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