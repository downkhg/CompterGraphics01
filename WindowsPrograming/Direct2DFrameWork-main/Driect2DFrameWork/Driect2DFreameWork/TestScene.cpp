#include "TestScene.h"
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
#include "DX2DClasses/Matrix3x3.h"
#include "DX2DClasses/Matrix3.h"
#include "DX2DClasses/UtilityDefine.h"
#include <conio.h>

using namespace DX2DClasses;

CTestScene::CTestScene()
{
	//Initialize();
}

CTestScene::~CTestScene()
{
	//Release();
}

void CTestScene::Initialize(HWND hWnd, CDriect2DFramwork* pDX2DFramework)
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();

	m_pColorBrushPalettet = new CColorBrushPalettet();
	m_pColorBrushPalettet->Initialize(pRenderTarget);
	
	m_pPlayer= new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(),6);
	m_pPlayer->ManualLoadImage(hWnd, L"Images\\player%02d.png");

	m_pOpossum = new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 6);
	m_pOpossum->ManualLoadImage(hWnd, L"Images\\opossum%02d.png");

	m_pEagle = new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 4);
	m_pEagle->ManualLoadImage(hWnd, L"Images\\eagle%02d.png");

	m_pCherry = new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 6);
	m_pCherry->ManualLoadImage(hWnd, L"Images\\cherry%02d.png");

	m_pGem = new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 5);
	m_pGem->ManualLoadImage(hWnd, L"Images\\gem%02d.png");

	m_pItemEffect = new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 4);
	m_pItemEffect->ManualLoadImage(hWnd, L"Images\\itemeffect%02d.png");

	m_pDeathEffect = new CImage(pDX2DFramework->GetD2DRenderTarget(), pDX2DFramework->GetImagingFactory(), 6);
	m_pDeathEffect->ManualLoadImage(hWnd, L"Images\\death%02d.png");

	m_pPlayerObject = new CGameObject();
	m_pPlayerObject->Initialize(m_pPlayer, true);

	m_pOpossumObject = new CGameObject();
	m_pOpossumObject->Initialize(m_pOpossum, true);

	MatrixTest();
}

void CTestScene::Release()
{
	m_pPlayerObject->Release();
	delete m_pPlayerObject;
	m_pOpossumObject->Release();
	delete m_pOpossumObject;

	delete m_pDeathEffect;
	delete m_pItemEffect;
	delete m_pGem;
	delete m_pCherry;
	delete m_pEagle;
	delete m_pOpossum;
	delete m_pPlayer;

	m_pColorBrushPalettet->Release();
	delete m_pColorBrushPalettet;
}

void CTestScene::Update()
{
	if (CInputManager::GetAsyncKeyStatePress(VK_RIGHT))
		m_vPos.x++;
	if (CInputManager::GetAsyncKeyStatePress(VK_LEFT))
		m_vPos.x--;
	if (CInputManager::GetAsyncKeyStatePress(VK_DOWN))
		m_vPos.y++;
	if (CInputManager::GetAsyncKeyStatePress(VK_UP))
		m_vPos.y--;
}

void CTestScene::Draw()
{
	ID2D1HwndRenderTarget* pRenderTarget = CSingletonRenderTarget::GetRenderTarget();
	static int nAniIdx = 0;
	static CAnimator2D cAnimator(m_pPlayer->GetAnimationCount());
	static float fAngle = 0;

	SVector2 vScale(1,1);
	SVector2 vSize = m_pPlayer->GetImageSize();
	SVector2 vTL_A = vSize;
	SVector2 vBR_A = vTL_A + vSize;
	SVector2 vTR_A(vBR_A.x, vTL_A.y);
	SVector2 vBL_A(vTL_A.x, vBR_A.y);

	SVector2 vTL_B;
	//SVector2 vTL_B = m_vPos;
	SVector2 vBR_B = vTL_B + vSize;
	SVector2 vTR_B(vBR_B.x, vTL_B.y);
	SVector2 vBL_B(vTL_B.x, vBR_B.y);

	CTransform& cTrnasform = m_pPlayerObject->GetTransform();

	cTrnasform.SetTransrate(m_vPos);
	SVector2 vAsix = vSize * 0.5f;
	//vAsix = m_vPos + vAsix;
	cTrnasform.SetAsixPoint(vAsix);
	cTrnasform.SetTRS(m_vPos, fAngle, vScale);
	//cTrnasform.Rotate(fAngle);
	//cTrnasform.Scale(SVector2(2, 2));
	m_pPlayerObject->Update();
	m_pOpossumObject->Update();

	m_pPlayer->DrawBitmap(vTL_A, vScale, 0, nAniIdx);
	m_pPlayer->DrawBitmap(m_vPos, vScale, 0, nAniIdx);
	m_pPlayer->DrawBitmap(cTrnasform.GetTransfrom(), nAniIdx);
	cAnimator.DrawImage(m_pPlayer, cTrnasform);
	m_pPlayerObject->Draw();
	m_pOpossum->DrawBitmap(SVector2(), vScale, 0, 0);
	m_pOpossumObject->Draw();

	cAnimator.UpdateFrame();
	
	CColorBrush* pRedBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::RED);
	CColorBrush* pGreenBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::GREEN);
	CColorBrush* pYellowBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::YELLOW);
	CColorBrush* pBlackBrush = m_pColorBrushPalettet->GetBrushClass(CColorBrushPalettet::BLACK);
	
	SVector2 vCirclePos = vTL_A + vAsix;
	float fCircleRadius = 10;
	SVector2 rect[] = { vTL_B, vTR_B, vBR_B, vBL_B };
	for (int i = 0; i < 4; i++)
		rect[i] = CTransform::MutipleVectorToMatrix(rect[i], cTrnasform.GetTransfrom());
	if (CCollisionCheck::OverlapCircleToOBB(vCirclePos, fCircleRadius, rect[0], rect[1], rect[2], rect[3]))
	{

	}

	SVector2 vRight(1, 0);
	SVector2 vUp(0, 1);
	SVector2 vRU(1, 1);
	vRU = vRU.Normalize();
	SVector2 vLineStart(100, 100);
	SVector2 vLineEnd;
	SVector2 vLineCenter;
	float fLineDist = 100;

	vLineEnd = vLineStart + vRight * fLineDist;
	CDebugHelper::DrawLine(vLineStart, vLineEnd, pRedBrush);
	vLineEnd = vLineStart + vUp * fLineDist;
	CDebugHelper::DrawLine(vLineStart, vLineEnd, pGreenBrush);
	SVector2 vCrossZRight = SVector2::CrossZ(vRight);
	vLineEnd = vLineStart + vCrossZRight * fLineDist;
	CDebugHelper::DrawLine(vLineStart, vLineEnd, pYellowBrush);
	SVector2 vCrossZUp = SVector2::CrossZ(vUp);
	vLineEnd = vLineStart + vCrossZUp * fLineDist;
	CDebugHelper::DrawLine(vLineStart, vLineEnd, pBlackBrush);
	vLineEnd = vLineStart + vRU * fLineDist;
	CDebugHelper::DrawLine(vLineStart, vLineEnd, pRedBrush);
	SVector2 vCrossZRU = SVector2::CrossZ(vRU);
	vLineEnd = vLineStart + vCrossZRU * fLineDist;
	CDebugHelper::DrawLine(vLineStart, vLineEnd, pYellowBrush);
	SVector2 vDir = m_vPos - vLineStart;
	vDir = vDir.Normalize();
	vLineEnd = vLineStart + vDir * fLineDist;
	CDebugHelper::DrawLine(vLineStart, m_vPos, pRedBrush);
	SVector2 vCrossZ = SVector2::CrossZ(vDir);
	vLineEnd = vLineStart + vCrossZ * fLineDist;
	CDebugHelper::DrawLine(vLineStart, vLineEnd, pYellowBrush);
	vLineCenter = vLineStart + vCrossZ * fLineDist*0.5f;
	vLineEnd = vLineStart + (SVector2::left() * fLineDist);
	CDebugHelper::DrawLine(vLineStart, vLineEnd, pBlackBrush);

	CDebugHelper::DrawCircle(m_vPos, 1, pRedBrush);
	int nCheck;
	

	if (CCollisionCheck::OverlapPointToOBB(vCirclePos, vTL_A, vTR_A, vBR_A, vBL_A))
	{
		CDebugHelper::DrawRect(vTL_A, vBR_A, pRedBrush);
		//CDebugHelper::DrawCircle(vCirclePos, fCircleRadius, pRedBrush);

	}
	else
	{
		CDebugHelper::DrawRect(vTL_A, vBR_A, pBlackBrush);
		CDebugHelper::DrawCircle(vCirclePos, fCircleRadius, pBlackBrush);
	}

	//if (CCollisionCheck::OverlapAABBtoCircle(vTL_A, vBR_A, vCirclePos, fCircleRadius))
	//{
	//	CDebugHelper::DrawRect(vTL_A, vBR_A, pRedBrush);
	//	CDebugHelper::DrawCircle(vCirclePos, fCircleRadius, pRedBrush);
	//	
	//}
	//else
	//{
	//	CDebugHelper::DrawRect(vTL_A, vBR_A, pBlackBrush);
	//	CDebugHelper::DrawCircle(vCirclePos, fCircleRadius, pBlackBrush);	
	//}

	if (CCollisionCheck::OverlapAABBtoAABB(vTL_A, vBR_A, vTL_B, vBR_B))
	{
		/*CDebugHelper::LogConsole("A: %s,", vTL_A.GetChar());
		CDebugHelper::LogConsole("%s\n", vBR_A.GetChar());
		CDebugHelper::LogConsole("B: %s,", vTL_B.GetChar());
		CDebugHelper::LogConsole("%s\n", vBR_B.GetChar());*/

		CDebugHelper::DrawRect(vTL_A, vBR_A, pRedBrush);
		CDebugHelper::DrawRect(vTL_B, vBR_B, pRedBrush);
	}
	else
	{
		CDebugHelper::DrawRect(vTL_A, vBR_A, pBlackBrush);
		CDebugHelper::DrawRect(vTL_B, vBR_B, pBlackBrush);
	}

	CCollisionCheck::OverlapPointToLine(m_vPos, vLineStart, vLineEnd, nCheck);

	switch (nCheck)
	{
	case CCollisionCheck::E_LINE_CHECK::UP:
		CDebugHelper::DrawLine(m_vPos, vLineCenter, pYellowBrush);
		break;
	case CCollisionCheck::E_LINE_CHECK::ON:
		CDebugHelper::DrawLine(m_vPos, vLineCenter, pGreenBrush);
		break;
	case CCollisionCheck::E_LINE_CHECK::DOWN:
		CDebugHelper::DrawLine(m_vPos, vLineCenter, pRedBrush);
		break;
	}

	if (nAniIdx < m_pPlayer->GetAnimationCount() - 1)
		nAniIdx++;
	else
		nAniIdx = 0;

	if (fAngle < 360)
		fAngle++;
	else
		fAngle = 0;
}

void CTestScene::MatrixTest()
{
	float fScale = 2;
	D2D1_POINT_2F sPos = D2D1::Point2F(100,100);
	D2D1_POINT_2F sTrans = D2D1::Point2F(50, 50);
	//D2D1::Matrix3x2F sMat = D2D1::Matrix3x2F::Identity();
	D2D1::Matrix3x2F sT = D2D1::Matrix3x2F::Translation(sTrans.x, sTrans.y);
	D2D1::Matrix3x2F sR= D2D1::Matrix3x2F::Rotation(90, D2D1::Point2F(0, 0));
	D2D1::Matrix3x2F sS = D2D1::Matrix3x2F::Scale(fScale, fScale);
	D2D1::Matrix3x2F sRTS = sR * sT * sS;
	D2D1_POINT_2F sResult = sPos * sRTS;
	//CDebugHelper::LogConsole("vResult:(%f,%f)\n", sResult.x, sResult.y);

	//CDebugHelper::LogConsole(SMatrix3x3::GetChar(sT, "sT"));
	//CDebugHelper::LogConsole(SMatrix3x3::GetChar(sR, "sR"));
	//CDebugHelper::LogConsole(SMatrix3x3::GetChar(sS, "sS"));
	CDebugHelper::LogConsole(SMatrix3x3::GetChar(sRTS,"sRTS"));
	CDebugHelper::LogConsole(SVector2::GetChar(sPos, "sPos"));
	CDebugHelper::LogConsole(SVector2::GetChar(sResult,"sResult"));

	Matrix3 mat3R, mat3T, mat3S, mat3RTS;

	mat3T = Matrix3::CreateTranslation(SVector2(sTrans));
	mat3R = Matrix3::CreateRotation(90 * DEG2RAD);
	mat3S = Matrix3::CreateScale(fScale);
	mat3RTS = mat3R * mat3T * mat3S;

	//CDebugHelper::LogConsole(mat3T.GetChar("mat3T"));
	//CDebugHelper::LogConsole(mat3R.GetChar("mat3R"));
	//CDebugHelper::LogConsole(mat3S.GetChar("mat3S"));
	CDebugHelper::LogConsole(mat3RTS.GetChar("mat3RTS"));

	SVector2 vPos(sPos);
	SMatrix3x3 matR, matT, matS, matRTS;
	matT.Translation(sTrans.x, sTrans.y);
	matR.Rotate(90);
	matS.Scale(fScale, fScale);
	matRTS = matR * matT * matS;
	//SVector2 vResult = matRTS * vPos;
	SVector2 vResult = vPos * matRTS;

	//CDebugHelper::LogConsole(matT.GetChar("matT"));
	//CDebugHelper::LogConsole(matR.GetChar("matR"));
	//CDebugHelper::LogConsole(matS.GetChar("matS"));
	CDebugHelper::LogConsole( matRTS.GetChar("matRTS") );
	CDebugHelper::LogConsole(vPos.GetChar("vPos"));
	CDebugHelper::LogConsole( vResult.GetChar("vResult"));


}