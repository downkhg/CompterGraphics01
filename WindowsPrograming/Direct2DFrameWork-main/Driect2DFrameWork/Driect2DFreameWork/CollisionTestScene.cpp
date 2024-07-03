#include "CollisionTestScene.h"
#include "DX2DClasses/Driect2DFramework.h"
#include "DX2DClasses/SingletonRenderTarget.h"
#include "DX2DClasses/ColorBrush.h"
#include "DX2DClasses/Vector2.h"
#include "DX2DClasses/Image.h"
#include "DX2DClasses/InputManager.h"
#include "DX2DClasses/CollisionCheck.h"
#include "DX2DClasses/ColorBrushPalettet.h"
#include "DX2DClasses/DebugHelper.h"
#include "DX2DClasses/GameObject.h"
#include "DX2DClasses/Colliders.h"
#include "DX2DClasses/Matrix3.h"
#include <conio.h>

using namespace DX2DClasses;

CollisionTestScene::CollisionTestScene()
{
	//Initialize();
}

CollisionTestScene::~CollisionTestScene()
{
	//Release();
}

void CollisionTestScene::Initialize(HWND hWnd, CDriect2DFramwork* pDX2DFramework)
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();

	m_pColorBrushPalettet = new CColorBrushPalettet();
	m_pColorBrushPalettet->Initialize(pRenderTarget);

	m_pPlayerImage = new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 6);
	m_pPlayerImage->ManualLoadImage(hWnd, L"Images\\player%02d.png");
	m_pPlayerObject = new CGameObject();
	m_pPlayerObject->Initialize(m_pPlayerImage, true);

	m_pOpossumImage = new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 6);
	m_pOpossumImage->ManualLoadImage(hWnd, L"Images\\opossum%02d.png");
	m_pOpossumObject = new CGameObject();
	m_pOpossumObject->Initialize(m_pOpossumImage, true);

	m_pEagleImage = new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 4);
	m_pEagleImage->ManualLoadImage(hWnd, L"Images\\eagle%02d.png");
	m_pEagleObject = new CGameObject();
	m_pEagleObject->Initialize(m_pEagleImage, true);

	m_pPlayerBoxCollider = new CBoxCollider();
	m_pPlayerBoxCollider->InitCollider(m_pPlayerObject->GetTransformPtr(), SVector2(), m_pPlayerImage->GetImageSize());
	m_pPlayerRectCollider = new CRectCollider();
	m_pPlayerRectCollider->InitCollider(m_pPlayerObject->GetTransformPtr(), SVector2(), m_pPlayerImage->GetImageSize());
	m_pPlayerCircleCollider = new CCircleCollider();
	m_pPlayerCircleCollider->InitCollider(m_pPlayerObject->GetTransformPtr(), SVector2(), m_pPlayerImage->GetImageSize());

	m_pOpossumBoxCollider = new CBoxCollider();
	m_pOpossumBoxCollider->InitCollider(m_pOpossumObject->GetTransformPtr(), SVector2(), m_pOpossumImage->GetImageSize());
	m_pOpossumRectCollider = new CRectCollider();
	m_pOpossumRectCollider->InitCollider(m_pOpossumObject->GetTransformPtr(), SVector2(), m_pOpossumImage->GetImageSize());
	m_pEagleCircleCollider = new CCircleCollider();
	m_pEagleCircleCollider->InitCollider(m_pEagleObject->GetTransformPtr(), SVector2(), m_pEagleImage->GetImageSize());
}

void CollisionTestScene::Release()
{
	delete m_pPlayerBoxCollider;
	delete m_pOpossumRectCollider;
	delete m_pEagleCircleCollider;

	m_pPlayerObject->Release();
	delete m_pPlayerObject;
	m_pOpossumObject->Release();
	delete m_pOpossumObject;
	m_pEagleObject->Release();
	delete m_pEagleObject;

	delete m_pPlayerImage;
	delete m_pOpossumImage;
	delete m_pEagleImage;

	m_pColorBrushPalettet->Release();
	delete m_pColorBrushPalettet;
}

void CollisionTestScene::Update()
{
	static SVector2 vPlayerPos;
	float fPlayerSpeed = 5;
	float fOpossumSpeed = 3;
	float fEangleSpeed = 1;
	//벡터방식 연산보다는 효률적이다.
	if (CInputManager::GetAsyncKeyStatePress(VK_RIGHT))
		vPlayerPos.x += fPlayerSpeed; //vPlayerPos = vPlayerPos + SVector2::right() * fPlayerSpeed;
	if (CInputManager::GetAsyncKeyStatePress(VK_LEFT))
		vPlayerPos = vPlayerPos + SVector2::left() * fPlayerSpeed;//vPlayerPos.x -= fPlayerSpeed;
	if (CInputManager::GetAsyncKeyStatePress(VK_DOWN))
		vPlayerPos = vPlayerPos + SVector2::down() * fPlayerSpeed;//vPlayerPos.y += fPlayerSpeed;
	if (CInputManager::GetAsyncKeyStatePress(VK_UP))
		vPlayerPos.y -= fPlayerSpeed;//vPlayerPos = vPlayerPos + SVector2::up() * fPlayerSpeed;

	static float fAngle = 0;
	{
		CTransform& cTrnasform = m_pPlayerObject->GetTransform();
		SVector2 vSize = m_pPlayerObject->GetImage()->GetImageSize();
		SVector2 vScale(1, 1);
		cTrnasform.SetTransrate(vPlayerPos);
		SVector2 vAsix = vSize * 0.5f;
		cTrnasform.SetAsixPoint(vAsix);
		cTrnasform.SetTRS(vPlayerPos, fAngle, vScale);
		if (fAngle > 360) fAngle -= 360; fAngle++;
	}
	m_pPlayerObject->Update();

	//static SVector2 vOpossumPos(1000, 0);
	
	static SVector2 vOpossumPos(100, 100);
	{
		static float fAngle = 0;
		CTransform& cTrnasform = m_pOpossumObject->GetTransform();
		SVector2 vSize = m_pOpossumObject->GetImage()->GetImageSize();
		SVector2 vScale(1, 1);
		cTrnasform.SetTransrate(vOpossumPos);
		SVector2 vAsix = vSize * 0.5f;
		cTrnasform.SetAsixPoint(vAsix);
		cTrnasform.SetTRS(vOpossumPos, fAngle, vScale);
		fAngle+=10;
	}
	//vOpossumPos = vOpossumPos + SVector2::left() * fOpossumSpeed;
	m_pOpossumObject->Update();

	static SVector2 vEaglePos(300, 300);
	{
		CTransform& cTrnasform = m_pEagleObject->GetTransform();
		SVector2 vSize = m_pEagleObject->GetImage()->GetImageSize();
		SVector2 vScale(1, 1);
		cTrnasform.SetTransrate(vEaglePos);
		SVector2 vAsix = vSize * 0.5f;
		cTrnasform.SetAsixPoint(vAsix);
		cTrnasform.SetTRS(vEaglePos, 0, vScale);

		SVector2 vDir = vPlayerPos - vEaglePos;
		vEaglePos = vEaglePos + vDir.Normalize() * fEangleSpeed;
	}
	m_pEagleObject->Update();
}

void CollisionTestScene::Draw()
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();

	m_pPlayerObject->Draw();

	m_pOpossumObject->Draw();

	m_pEagleObject->Draw();

	//CollisionCheckDraw(); //오브젝트 드로우와 함깨 사용시 포인트들의 위치가 어긋남
	ColliderDraw();
}

void CollisionTestScene::CollisionCheckDraw()
{
	CDebugHelper::LogConsole("CollisionCheckProccess 1\n");
	CColorBrush* pRedBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::RED);
	CColorBrush* pGreenBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::GREEN);
	CColorBrush* pYellowBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::YELLOW);
	CColorBrush* pBlackBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::BLACK);

	SVector2 vPlayerSize = m_pPlayerObject->GetImage()->GetImageSize();
	SVector2 vPlayerSizeHalf = vPlayerSize * 0.5f;
	SVector2 vPlayerPos = m_pPlayerObject->GetTransform().GetTransrate();
	/*SVector2 vPlayerTopLeft = vPlayerPos;
	SVector2 vPlayerBottomRight = vPlayerPos + vPlayerSize;
	SVector2 vPlayerTopRight = SVector2(vPlayerTopLeft.x,vPlayerBottomRight.y);
	SVector2 vPlayerBottomLeft = SVector2(vPlayerBottomRight.x,vPlayerTopLeft.y);*/
	SVector2 vPlayerTopLeft = m_pPlayerBoxCollider->GetWorldTL();
	SVector2 vPlayerBottomRight = m_pPlayerBoxCollider->GetWorldBR();
	SVector2 vPlayerTopRight = m_pPlayerBoxCollider->GetWorldTR();
	SVector2 vPlayerBottomLeft = m_pPlayerBoxCollider->GetWorldBL();

	/*SVector2 vOpossumSize = m_pOpossumObject->GetImage()->GetImageSize();
	SVector2 vOpossumPos = m_pOpossumObject->GetTransform().GetTransrate();
	SVector2 vOpossumTopLeft = vOpossumPos;
	SVector2 vOpossumBottomRight = vOpossumPos + vOpossumSize;*/

	SVector2 vEagleSize = m_pEagleObject->GetImage()->GetImageSize();
	SVector2 vEaglePos = m_pEagleObject->GetTransform().GetTransrate() + vEagleSize * 0.5f;
	float fEagleRad = vEagleSize.Magnitude() * 0.3f;

	//플레이어와 주머니쥐를 OBB로 놓고 계산하기
	SVector2 vOpossumTopLeft = m_pOpossumBoxCollider->GetWorldTL();
	SVector2 vOpossumTopRight = m_pOpossumBoxCollider->GetWorldTR();
	SVector2 vOpossumBottomRight = m_pOpossumBoxCollider->GetWorldBR();
	SVector2 vOpossumBottomLeft = m_pOpossumBoxCollider->GetWorldBL();

	/*if (CCollisionCheck::OverlapOBBtoOBB(vPlayerTopLeft, vPlayerTopRight, vPlayerBottomRight, vPlayerBottomLeft,
										 vOpossumTopLeft, vOpossumTopRight, vOpossumBottomRight, vOpossumBottomLeft))
	{
		CDebugHelper::DrawLine(vPlayerTopLeft, vPlayerTopRight, pRedBrush);
		CDebugHelper::DrawLine(vPlayerTopRight, vPlayerBottomRight, pRedBrush);
		CDebugHelper::DrawLine(vPlayerBottomRight, vPlayerBottomLeft, pRedBrush);
		CDebugHelper::DrawLine(vPlayerBottomLeft, vPlayerTopLeft, pRedBrush);
	}
	else
	{
		CDebugHelper::DrawLine(vPlayerTopLeft, vPlayerTopRight, pBlackBrush);
		CDebugHelper::DrawLine(vPlayerTopRight, vPlayerBottomRight, pBlackBrush);
		CDebugHelper::DrawLine(vPlayerBottomRight, vPlayerBottomLeft, pBlackBrush);
		CDebugHelper::DrawLine(vPlayerBottomLeft, vPlayerTopLeft, pBlackBrush);
	}*/

	//CDebugHelper::DrawRect(vOpossumTopLeft, vOpossumBottomRight, pBlackBrush);
	
	/*SVector2 vOpossumTopLeft = m_pOpossumRectCollider->GetWorldTL();
	SVector2 vOpossumBottomRight = m_pOpossumRectCollider->GetWorldBR();*/

	/*if (CCollisionCheck::OverlapAABBtoOBB(vOpossumTopLeft, vOpossumBottomRight, vPlayerTopLeft, vPlayerTopRight, vPlayerBottomRight, vPlayerBottomLeft))
	{
		CDebugHelper::DrawLine(vPlayerTopLeft, vPlayerTopRight, pRedBrush);
		CDebugHelper::DrawLine(vPlayerTopRight, vPlayerBottomRight, pRedBrush);
		CDebugHelper::DrawLine(vPlayerBottomRight, vPlayerBottomLeft, pRedBrush);
		CDebugHelper::DrawLine(vPlayerBottomLeft, vPlayerTopLeft, pRedBrush);
	}
	else
	{
		CDebugHelper::DrawLine(vPlayerTopLeft, vPlayerTopRight, pBlackBrush);
		CDebugHelper::DrawLine(vPlayerTopRight, vPlayerBottomRight, pBlackBrush);
		CDebugHelper::DrawLine(vPlayerBottomRight, vPlayerBottomLeft, pBlackBrush);
		CDebugHelper::DrawLine(vPlayerBottomLeft, vPlayerTopLeft, pBlackBrush);
	}*/
	////독수리와 주머니쥐 충돌체크
	//if (CCollisionCheck::OverlapCircleToAABB(vEaglePos, fEagleRad, vOpossumTopLeft, vOpossumBottomRight))
	//{
	//	CDebugHelper::DrawRect(vOpossumTopLeft, vOpossumBottomRight, pRedBrush);
	//}
	//else
	//{
	//	CDebugHelper::DrawRect(vOpossumTopLeft, vOpossumBottomRight, pBlackBrush);
	//}

	////플레이어와 독수리 충돌체크
	//if (CCollisionCheck::OverlapCircleToOBB(vEaglePos, fEagleRad, vPlayerTopLeft, vPlayerTopRight, vPlayerBottomRight, vPlayerBottomLeft))
	//{
	//	CDebugHelper::DrawCircle(vEaglePos, fEagleRad, pRedBrush);
	//}
	//else
	//{
	//	CDebugHelper::DrawCircle(vEaglePos, fEagleRad, pBlackBrush);
	//}

	CDebugHelper::LogConsole("CollisionCheckProccess 2\n");
}

void CollisionTestScene::ColliderDraw()
{
	CColorBrush* pRedBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::RED);
	CColorBrush* pGreenBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::GREEN);
	CColorBrush* pYellowBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::YELLOW);
	CColorBrush* pBlackBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::BLACK);

	////플레이어 충돌테스트
	//if (m_pPlayerBoxCollider->ToRect(m_pOpossumRectCollider))
	//	m_pPlayerBoxCollider->DrawOutline(pRedBrush);
	//else
	//	m_pPlayerBoxCollider->DrawOutline(pBlackBrush);

	//if (m_pPlayerBoxCollider->ToBox(m_pOpossumBoxCollider))
	//	m_pPlayerBoxCollider->DrawOutline(pRedBrush);
	//else
	//	m_pPlayerBoxCollider->DrawOutline(pBlackBrush);

	//if(m_pPlayerBoxCollider->ToCircle(m_pEagleCircleCollider))
	//	m_pPlayerBoxCollider->DrawOutline(pRedBrush);
	//else
	//	m_pPlayerBoxCollider->DrawOutline(pBlackBrush);
	//m_pPlayerBoxCollider->DrawOutline(pBlackBrush);

	//주머니쥐 충돌테스트
	if (m_pOpossumBoxCollider->ToRect(m_pPlayerRectCollider))
		m_pOpossumBoxCollider->DrawOutline(pRedBrush);
	else
		m_pOpossumBoxCollider->DrawOutline(pBlackBrush);

	if (m_pOpossumBoxCollider->ToBox(m_pPlayerBoxCollider))
		m_pOpossumBoxCollider->DrawOutline(pRedBrush);
	else
		m_pOpossumBoxCollider->DrawOutline(pBlackBrush);

	if (m_pOpossumBoxCollider->ToCircle(m_pEagleCircleCollider))
		m_pOpossumBoxCollider->DrawOutline(pRedBrush);
	else
		m_pOpossumBoxCollider->DrawOutline(pBlackBrush);

	//m_pOpossumBoxCollider->DrawOutline(pBlackBrush);

	////독수리 충돌테스트
	//if(m_pEagleCircleCollider->ToRect(m_pOpossumRectCollider))
	//	m_pEagleCircleCollider->DrawOutline(pBlackBrush);
	//else
	//	m_pEagleCircleCollider->DrawOutline(pRedBrush);

	//if (m_pEagleCircleCollider->ToBox(m_pPlayerBoxCollider))
	//	m_pEagleCircleCollider->DrawOutline(pBlackBrush);
	//else
	//	m_pEagleCircleCollider->DrawOutline(pRedBrush);

	//if(m_pEagleCircleCollider->ToCircle(m_pPlayerCircleCollider))
	//	m_pEagleCircleCollider->DrawOutline(pBlackBrush);
	//else
	//	m_pEagleCircleCollider->DrawOutline(pRedBrush);

	//m_pEagleCircleCollider->DrawOutline(pBlackBrush);
	
}