/*##################################
Direct2D(수업용)
파일명: Driect2D.cpp (원본프로젝트 및 소스제공: http://vsts2010.tistory.com/) 
작성자: 김홍규 (downkhg@gmail.com)
마지막수정날짜: 2019.12.03
버전: 1.1
###################################*/


// Direct2D.cpp: 응용 프로그램의 진입점을 정의합니다.

#include "stdafx.h"
#include "Direct2D.h"
#include <string.h>
#include <conio.h>
#include "Image.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[MAX_LOADSTRING];                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WindowProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//	최대 로드할 수 있는 이미지 갯수.
#define _MAX_IMAGE_COUNT_ 1024
TCHAR**						g_ppImageNameArray = new TCHAR*[_MAX_IMAGE_COUNT_];
int							g_iImageCount = 0;

D2D1_POINT_2F g_sPoint;
D2D1_POINT_2F g_sPointSize;

void InitializeD2D(); //D2D초기화
void InitializeRect(HWND); //랜더링할 뷰포트 타겟 생성
void CreateD2DBitmapFromFile(HWND, TCHAR*, IWICFormatConverter*&, ID2D1Bitmap*&); //이미지 생성하기
void ManualLoadImage(HWND, const TCHAR*, IWICFormatConverter**, ID2D1Bitmap**);//이미지 여러개 읽기

ID2D1Factory*				g_ipD2DFactory = nullptr;
ID2D1HwndRenderTarget*		g_ipRT = nullptr;
IWICImagingFactory*			g_ipWICFactory = nullptr;

IWICFormatConverter*		g_ipConvertedSrcBmp[ANI_SIZE]; //포맷변환기
ID2D1Bitmap*				g_ipD2DBitmap[ANI_SIZE]; //비트맵

IWICFormatConverter* g_ipConvertedSrcBmp2[ANI_SIZE]; //포맷변환기
ID2D1Bitmap* g_ipD2DBitmap2[ANI_SIZE]; //비트맵

int g_nAniIdx = 0;
int g_nAniIdx2 = 6;

ID2D1SolidColorBrush*	g_ipBlackBrush = nullptr;

float g_fRotAngle;

CImage g_cImage;


D2D1_RECT_F TrasformArea(D2D1_POINT_2F pos, D2D1_POINT_2F size, D2D1::Matrix3x2F mat)
{
	D2D1_POINT_2F sTL = D2D1::Point2F(pos.x, pos.y);
	D2D1_POINT_2F sTR = D2D1::Point2F(pos.x+size.x, pos.y);
	D2D1_POINT_2F sBL = D2D1::Point2F(pos.x, pos.y + size.y);
	D2D1_POINT_2F sBR = D2D1::Point2F(pos.x+size.x, pos.y + size.y);

	D2D1_POINT_2F sRectTL = sTL * mat;
	D2D1_POINT_2F sRectBR = sBR * mat;

	return D2D1::RectF(sRectTL.x, sRectTL.y, sRectBR.x, sRectBR.y);
}

void Draw()
{
	g_ipRT->BeginDraw();
	g_ipRT->Clear(D2D1::ColorF(D2D1::ColorF::Blue));

	HRESULT hr = E_FAIL;

	//DrawBitmap(g_ipD2DBitmap2[g_nAniIdx], D2D1::Point2F(0,0), g_sPointSize, 0);
	//DrawBitmap(g_ipD2DBitmap[g_nAniIdx], g_sPoint, g_sPointSize, 0);

	g_cImage.DrawBitmap(g_sPoint, D2D1::Point2F(1,1), 0, g_nAniIdx);
	

	D2D1_POINT_2F vSize = g_cImage.GetSize();
	D2D1::Matrix3x2F matT = D2D1::Matrix3x2F::Translation(g_sPoint.x + 100, g_sPoint.y);
	D2D1::Matrix3x2F matR = D2D1::Matrix3x2F::Rotation(20, g_cImage.GetCenter());
	D2D1::Matrix3x2F matS = D2D1::Matrix3x2F::Scale(2, 2);
	D2D1::Matrix3x2F matTRS = matT * matR * matS; //회전이 이동후에 일어
	D2D1::Matrix3x2F matRTS = matR * matT * matS; //회전이 먼저일어나므로 조작하는데 어려움이 없다.
	g_cImage.DrawBitmap(matTRS, g_nAniIdx2);
	

	if (g_nAniIdx < ANI_SIZE - 1)
		g_nAniIdx++;
	else
		g_nAniIdx = 0;

	if (g_nAniIdx2 < ANI_SIZE - 1)
		g_nAniIdx2++;
	else
		g_nAniIdx2 = 0;

	g_fRotAngle += 1.0f;

	_cprintf("Anlge: %f\n",g_fRotAngle);

	hr = g_ipRT->EndDraw();
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	AllocConsole(); //콘솔화면 만들기

	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DIRECT2D, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 응용 프로그램 초기화를 수행합니다.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DIRECT2D));

    MSG msg;

    // 기본 메시지 루프입니다.
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	assert(hr == S_OK);
	FreeConsole(); //콘솔화면 끄기

    return (int) msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WindowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DIRECT2D));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DIRECT2D);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

    switch (message)
    {
	case WM_CREATE:
		InitializeD2D();
		InitializeRect(hWnd);

		ManualLoadImage(hWnd, L"Images\\sonic_%04d.png",g_ipConvertedSrcBmp,g_ipD2DBitmap);
		ManualLoadImage(hWnd, L"Images\\red_%04d.png", g_ipConvertedSrcBmp2, g_ipD2DBitmap2);
		g_cImage.m_ipWICFactory = g_ipWICFactory;
		g_cImage.m_ipRT = g_ipRT;
		g_cImage.ManualLoadImage(hWnd, L"Images\\red_%04d.png");
		SetTimer(hWnd, 1, 10, NULL); //타이머 시작
		break;
	case WM_TIMER:
		InvalidateRect(hWnd,NULL,FALSE); //타이머 호출시 마다, 윈도우영역을 초기화
		break;
	case WM_KEYDOWN:
		switch (wParam) {
		case VK_LEFT:
			g_sPoint.x--;
			break;
		case VK_RIGHT:
			g_sPoint.x++;
			break;
		case VK_UP:
			g_sPoint.y--;
			break;
		case VK_DOWN:
			g_sPoint.y++;
			break;
		case VK_SPACE:
			
			break;
		}
		return 0;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 메뉴 선택을 구문 분석합니다.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		Draw();
		EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

void InitializeD2D(void)
{
	HRESULT hr = E_FAIL;

	//-----------------------------------------------------------------------
	//	D2D Factory 를 생성한다.
	//-----------------------------------------------------------------------
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,&g_ipD2DFactory);
	assert(hr == S_OK);
\
	//-----------------------------------------------------------------------
	//	Windows Imaging Component Factory 를 생성한다.
	//-----------------------------------------------------------------------
	hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&g_ipWICFactory));

	assert(hr == S_OK);

}

void CreateD2DBitmapFromFile(HWND hWnd, TCHAR* pImageFullPath, IWICFormatConverter* &ipConvertedSrcBmp, ID2D1Bitmap* &ipD2DBitmap)
{
	assert(pImageFullPath != nullptr);
	//assert(g_ipWICFactory != nullptr);
	//assert(g_ipRT != nullptr);

	HRESULT hr = E_FAIL;

	//----------------------------------------------------------------
	//	디코더를 생성한다.
	//----------------------------------------------------------------
	IWICBitmapDecoder* ipDecoderPtr = nullptr;
	hr = g_ipWICFactory->CreateDecoderFromFilename(pImageFullPath, nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&ipDecoderPtr);
	assert(hr == S_OK);

	//----------------------------------------------------------------
	//	디코더에서 프레임을 얻는다.
	//----------------------------------------------------------------
	IWICBitmapFrameDecode* ipFramePtr = nullptr;
	hr = ipDecoderPtr->GetFrame(0, &ipFramePtr);
	assert(hr == S_OK);


	//----------------------------------------------------------------
	//	프레임을 기반으로 해서 포맷 컨버터를 만든다.
	//----------------------------------------------------------------
	SafeRelease(ipConvertedSrcBmp);
	hr = g_ipWICFactory->CreateFormatConverter(&ipConvertedSrcBmp);
	assert(hr == S_OK);


	hr = ipConvertedSrcBmp->Initialize(ipFramePtr, GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom);
	assert(hr == S_OK);

	//----------------------------------------------------------------
	//	컨버트된 데이터를 기반으로 해서 실제 비트맵을 만든다.
	//----------------------------------------------------------------
	SafeRelease(ipD2DBitmap);
	hr = g_ipRT->CreateBitmapFromWicBitmap(ipConvertedSrcBmp, nullptr, &ipD2DBitmap);
	assert(hr == S_OK);

	SafeRelease(ipDecoderPtr);
	SafeRelease(ipFramePtr);
}


void InitializeRect(HWND hWnd)
{
	assert(g_ipRT == nullptr);
	assert(hWnd != 0);

	HRESULT hr = E_FAIL;
	RECT rc;
	GetClientRect(hWnd, &rc);

	D2D1_RENDER_TARGET_PROPERTIES dxRTProperties = D2D1::RenderTargetProperties();
	D2D1_SIZE_U dxSize = D2D1::SizeU(rc.right - rc.left,rc.bottom - rc.top);

	hr = g_ipD2DFactory->CreateHwndRenderTarget(dxRTProperties,D2D1::HwndRenderTargetProperties(hWnd, dxSize),&g_ipRT);

	hr = g_ipRT->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &g_ipBlackBrush);

	assert(hr == S_OK);
}

void ManualLoadImage(HWND hWnd, const TCHAR* format, IWICFormatConverter** ipConvertedSrcBmp, ID2D1Bitmap** ipD2DBitmap)
{
	TCHAR szFullPath[1024];

	for(int i =0; i< ANI_SIZE; i++)
	{ 
		wsprintf(szFullPath, format, i);
		CreateD2DBitmapFromFile(hWnd, szFullPath, ipConvertedSrcBmp[i], ipD2DBitmap[i]);

		RECT rc;
		g_sPointSize = D2D1::Point2F(ipD2DBitmap[i]->GetSize().width, ipD2DBitmap[i]->GetSize().height);
		SetRect(&rc, 100, 100, g_sPointSize.x, g_sPointSize.y);
		InvalidateRect(hWnd, &rc, TRUE);
	}
}


