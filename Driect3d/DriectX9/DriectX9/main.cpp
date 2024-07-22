/**----------------------------------------------------------------------------
 * \brief �޽� ���
 * ����: Meshes.cpp
 *
 * ����: ���� ���� ���������� ����ϱ� ���ؼ��� �������� 3D�𵨷��� ���� ����
 *       ���Ϸ� �о���̴� ���� �Ϲ����̴�. ���ེ���Ե� D3DX���� ������ X����
 *       ó������� �־ ��������,�ε������� �������� ���� �κ��� ������ش�.
 *       �̹� ������ D3DXMESH�� ����Ͽ� ������ �о �� ���ϰ� ������ ������
 *       �ؽ��ĸ� �Բ� ����ϴ� ���� �˾ƺ���.
 *
 *       �̹��� �Ұ������� ������ ���߿� ����ϰԵ� ������ ����߿� �ϳ���
 *       FVF�� �����Ͽ� ���ο� �޽ø� ����(clone)�ϴ� ���̴�. �� ����� ����Ͽ�
 *       �ؽ��� ��ǥ�� ��ֺ��͵��� �����޽ÿ� �߰��� ���ο� �޽ø� ������ �� �ִ�.
 *       (���߿� Cg,HLSL���� �����Ҷ� ���� ���� ���̴�.)
 *------------------------------------------------------------------------------
 */
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>




 /**-----------------------------------------------------------------------------
  *  ��������
  *------------------------------------------------------------------------------
  */
LPDIRECT3D9             g_pD3D = NULL; /// D3D ����̽��� ������ D3D��ü����
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; /// �������� ���� D3D����̽�

LPD3DXMESH              g_pMesh = NULL; // �޽� ��ü
D3DMATERIAL9* g_pMeshMaterials = NULL; // �޽ÿ��� ����� ����
LPDIRECT3DTEXTURE9* g_pMeshTextures = NULL; // �޽ÿ��� ����� �ؽ���
DWORD                   g_dwNumMaterials = 0L;   // �޽ÿ��� ������� ������ ����




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
    /// ������ ������Ʈ�� �׸����̱⶧����, �̹����� Z���۰� �ʿ��ϴ�.
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory(&d3dpp, sizeof(d3dpp));
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

    /// ����̽� ����
    if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, &g_pd3dDevice)))
    {
        return E_FAIL;
    }

    /// Z���۱���� �Ҵ�.
    g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

    /// �ֺ��������� �ִ����
    g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

    return S_OK;
}




/**-----------------------------------------------------------------------------
 * �������� �ʱ�ȭ
 * �޽��б�, ������ �ؽ��� �迭 ����
 *------------------------------------------------------------------------------
 */
HRESULT InitGeometry()
{
    /// ������ �ӽ÷� ������ ���ۼ���
    LPD3DXBUFFER pD3DXMtrlBuffer;
    //x���Ͽ� ������ ���ؽ�����,�ε�������,�ؽ�óUV���� ������ ���Եȴ�. 
    /// Tiger.x������ �޽÷� �о���δ�. �̶� ���������� �Բ� �д´�.
    if (FAILED(D3DXLoadMeshFromX("Tiger.x", D3DXMESH_SYSTEMMEM,
        g_pd3dDevice, NULL,
        &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
        &g_pMesh)))
    {
        /// ���� ������ ������ ������ �������� �˻�
        if (FAILED(D3DXLoadMeshFromX("..\\Tiger.x", D3DXMESH_SYSTEMMEM,
            g_pd3dDevice, NULL,
            &pD3DXMtrlBuffer, NULL, &g_dwNumMaterials,
            &g_pMesh)))
        {
            MessageBox(NULL, "Could not find tiger.x", "Meshes.exe", MB_OK);
            return E_FAIL;
        }
    }

    /// ���������� �ؽ��� ������ ���� �̾Ƴ���.
    D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
    g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];			/// ����������ŭ ��������ü �迭 ����
    g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];	/// ����������ŭ �ؽ��� �迭 ����

    for (DWORD i = 0; i < g_dwNumMaterials; i++)
    {
        /// ���������� ����
        g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

        /// �ֺ����������� Diffuse������
        g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;

        g_pMeshTextures[i] = NULL;
        if (d3dxMaterials[i].pTextureFilename != NULL &&
            lstrlen(d3dxMaterials[i].pTextureFilename) > 0)
        {
            /// �ؽ��ĸ� ���Ͽ��� �ε��Ѵ�
            if (FAILED(D3DXCreateTextureFromFile(g_pd3dDevice,
                d3dxMaterials[i].pTextureFilename,
                &g_pMeshTextures[i])))
            {
                /// �ؽ��İ� ���� ������ ������ �������� �˻�
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

    /// �ӽ÷� ������ �������� �Ұ�
    pD3DXMtrlBuffer->Release();

    return S_OK;
}




/**-----------------------------------------------------------------------------
 * �ʱ�ȭ�� ��ü�� �Ұ�
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
 * ��� ����
 *------------------------------------------------------------------------------
 */
VOID SetupMatrices()
{
    /// �������
    D3DXMATRIXA16 matWorld;
    D3DXMatrixRotationY(&matWorld, timeGetTime() / 1000.0f);
    g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

    /// ������� ����
    D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
    D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
    D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
    g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

    /// �������� ��� ����
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4, 1.0f, 1.0f, 100.0f);
    g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}




/**-----------------------------------------------------------------------------
 * ȭ�� �׸���
 *------------------------------------------------------------------------------
 */
VOID Render()
{
    /// �ĸ���ۿ� Z���۸� �����.
    g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

    /// ������ ����
    if (SUCCEEDED(g_pd3dDevice->BeginScene()))
    {
        /// ����,��,�������� ����� �����Ѵ�.
        SetupMatrices();

        /// �޽ô� ������ �ٸ� �޽ú��� �κ������� �̷�� �ִ�.
        /// �̵��� ������ �����ؼ� ��� �׷��ش�.
        for (DWORD i = 0; i < g_dwNumMaterials; i++)
        {
            /// �κ����� �޽��� ������ �ؽ��� ����
            g_pd3dDevice->SetMaterial(&g_pMeshMaterials[i]);
            g_pd3dDevice->SetTexture(0, g_pMeshTextures[i]);

            /// �κ����� �޽� ���
            g_pMesh->DrawSubset(i);
        }

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
    HWND hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial 06: Meshes",
        WS_OVERLAPPEDWINDOW, 100, 100, 300, 300,
        GetDesktopWindow(), NULL, wc.hInstance, NULL);

    /// Direct3D �ʱ�ȭ
    if (SUCCEEDED(InitD3D(hWnd)))
    {
        /// �������� �ʱ�ȭ
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



