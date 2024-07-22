/**----------------------------------------------------------------------------
* \brief ���� ����
* ����: Lights.cpp
*
* ����: ������ ����ϸ� �ξ��� ������ ������ �����ϴ�.
*       ������ ����ϱ� ���ؼ��� �����̳� ������ �����ؾ��Ѵ�. ���� ����������
*       ��ֺ��������� �����ϰ� �־�� �Ѵ�.
*       ������ ��ġ,����,������� ������ �������� �����ȴ�.
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
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; /// ������ ������ ��������

/// ����� ������ ������ ����ü
/// ������ ����ϱ⶧���� ��ֺ��Ͱ� �־�� �Ѵٴ� ����� �������.
struct CUSTOMVERTEX
{
	D3DXVECTOR3 position; /// ������ 3���� ��ǥ
	D3DXVECTOR3 normal;   /// ������ ��� ����
};

/// ����� ���� ����ü�� ���� ������ ��Ÿ���� FVF��
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)




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


	//g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW); //�ø����Ҵ�.
	g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); //�ø�������.

	//����ť�� �̿��Ͽ� ����ָ��� ��ü�� ���� �������Ѵ�.
	//g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE); // Z���۱���� �Ҵ�.
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE); // Z���۱���� ����.

	return S_OK;
}




/**-----------------------------------------------------------------------------
* �������� �ʱ�ȭ
*------------------------------------------------------------------------------
*/
HRESULT InitGeometry()
{
	//���� �ݻ�Ǿ� ������ �������� ���̱⶧���� ���󿡴��� ������ ����.
	/// �������� ����
	if (FAILED(g_pd3dDevice->CreateVertexBuffer(50 * 2 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	/// �˰����� ����ؼ� �Ǹ���(�� �Ʒ��� ���� ����)�� �����.
	CUSTOMVERTEX* pVertices;
	if (FAILED(g_pVB->Lock(0, 0, (void**)&pVertices, 0)))
		return E_FAIL;
	for (DWORD i = 0; i < 50; i++)
	{
		FLOAT theta = (2 * D3DX_PI * i) / (50 - 1);
		pVertices[2 * i + 0].position = D3DXVECTOR3(sinf(theta), -1.0f, cosf(theta));	/// �Ǹ����� �Ʒ��� ������ ��ǥ
		pVertices[2 * i + 0].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));	/// �Ǹ����� �Ʒ��� ������ ���
		pVertices[2 * i + 1].position = D3DXVECTOR3(sinf(theta), 1.0f, cosf(theta));	/// �Ǹ����� ���� ������ ��ǥ
		pVertices[2 * i + 1].normal = D3DXVECTOR3(sinf(theta), 0.0f, cosf(theta));	/// �Ǹ����� ���� ������ ���
	}
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
*------------------------------------------------------------------------------
*/
VOID SetupMatrices()
{
	/// �������
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);							/// ��������� ����������� ����
	D3DXMatrixRotationX(&matWorld, timeGetTime() / 500.0f);		/// X���� �߽����� ȸ����� ����
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);		/// ����̽��� ������� ����

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
* ���� ����
*------------------------------------------------------------------------------
*/
VOID SetupLights()
{
	/// ����(material)����
	/// ������ ����̽��� �� �ϳ��� ������ �� �ִ�.
	D3DMATERIAL9 mtrl;
	ZeroMemory(&mtrl, sizeof(D3DMATERIAL9));
	//�ֺ���: �ֺ��� ��ü�� ���缭 ��Ÿ�� ��
	mtrl.Ambient.r = 0.0f;
	mtrl.Ambient.g = 0.0f;
	mtrl.Ambient.b = 1.0f;
	mtrl.Ambient.a = 1.0f;
	//Ȯ�걤: ��ü�� ���� �ݻ�Ǿ� ��Ÿ�� ��(����)
	mtrl.Diffuse.r = 1.0f;
	mtrl.Diffuse.g = 0.0f;
	mtrl.Diffuse.b = 0.0f;
	mtrl.Diffuse.a = 1.0f;
	g_pd3dDevice->SetMaterial(&mtrl);

	/// ���� ����
	D3DXVECTOR3 vecDir;									/// ���⼺ ����(directional light)�� ���� ���� ����
	D3DLIGHT9 light;									/// ���� ����ü
	ZeroMemory(&light, sizeof(D3DLIGHT9));			/// ����ü�� 0���� �����.
	light.Type = D3DLIGHT_DIRECTIONAL;			/// ������ ����(�� ����,���⼺ ����,����Ʈ����Ʈ)
	light.Diffuse.r = 1.0f;							/// ������ ����� ���
	light.Diffuse.g = 1.0f;
	light.Diffuse.b = 1.0f;
	/*vecDir = vecDir(cosf(timeGetTime() / 350.0f),	/// ������ ����
		1.0f,
		sinf(timeGetTime() / 350.0f));*/
	vecDir = D3DXVECTOR3(0, 0, 1);
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vecDir);	/// ������ ������ �������ͷ� �����.

	light.Range = 1000.0f;									/// ������ �ٴٸ��� �ִ� �ִ�Ÿ�
	g_pd3dDevice->SetLight(0, &light);							/// ����̽��� 0�� ���� ��ġ
	g_pd3dDevice->LightEnable(0, TRUE);							/// 0�� ������ �Ҵ�
	g_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);			/// ���������� �Ҵ�

	g_pd3dDevice->SetRenderState(D3DRS_AMBIENT, 0x00202020);		/// ȯ�汤��(ambient light)�� �� ����
}




/**-----------------------------------------------------------------------------
* ȭ�� �׸���
*------------------------------------------------------------------------------
*/
VOID Render()
{
	/// �ĸ���ۿ� Z���۸� �����.
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	/// ������ ����
	if (SUCCEEDED(g_pd3dDevice->BeginScene()))
	{
		/// ������ ���� ����
		SetupLights();

		/// ����,��,�������� ����� �����Ѵ�.
		SetupMatrices();

		/// ���������� ������ �׸���.
		g_pd3dDevice->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2 * 50 - 2);

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
	HWND hWnd = CreateWindow("D3D Tutorial", "D3D Tutorial 04: Lights",
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



