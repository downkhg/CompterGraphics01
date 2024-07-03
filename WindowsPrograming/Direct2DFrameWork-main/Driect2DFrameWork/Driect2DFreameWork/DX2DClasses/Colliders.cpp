#include "Colliders.h"
#include "CollisionCheck.h"
#include "ColorBrush.h"
#include "GameObject.h"
#include "SingletonRenderTarget.h"
#include "DebugHelper.h"
#include <d2d1.h>
#include <d2d1_1helper.h>

using namespace DX2DClasses;

CCircleCollider::CCircleCollider(CTransform* pTransform, SVector2 pos, float radius)
{
	m_pTransform = pTransform;
	m_vPos = pos;
	m_fRadius = radius;
}

CCircleCollider::CCircleCollider(CTransform* pTransform, SVector2 pos, SVector2 size, float scale)
{
	m_pTransform = pTransform;
	m_vPos = pos;
	m_fRadius = size.Magnitude()*scale;
}

void CCircleCollider::InitCollider(CTransform* pTransform, SVector2 pos, SVector2 size, float scale)
{
	m_pTransform = pTransform;
	m_vPos = pos;
	m_fRadius = size.Magnitude() * scale;
}

SVector2& CCircleCollider::GetWorldPos()
{
	static SVector2 vPos;
	vPos = CTransform::MutipleVectorToMatrix(m_vPos, m_pTransform->GetTransfromPtr());
	return vPos;
}

void CCircleCollider::DrawOutline(CColorBrush* pColorBrush, float stroke)
{
	return CDebugHelper::DrawCircle(GetWorldPos(), m_fRadius, pColorBrush, stroke);
}

bool CCircleCollider::ToPoint(SVector2& pos)
{
	return CCollisionCheck::OverlapPointToCircle(pos, GetWorldPos(), m_fRadius);
}

bool CCircleCollider::ToRect(CRectCollider* pRect)
{
	return CCollisionCheck::OverlapCircleToAABB(GetWorldPos(), m_fRadius, pRect->GetWorldTL(), pRect->GetWorldBR());
}

bool CCircleCollider::ToBox(CBoxCollider* pBox)
{
	return CCollisionCheck::OverlapCircleToOBB(GetWorldPos(), m_fRadius, pBox->GetWorldTL(), pBox->GetWorldTR(), pBox->GetWorldBR(), pBox->GetWorldBL());
}

bool CCircleCollider::ToCircle(CCircleCollider* pCircle)
{
	return CCollisionCheck::OverlapCircleToCircle(GetWorldPos(), m_fRadius, pCircle->GetWorldPos(), pCircle->GetRadius());
}

CRectCollider::CRectCollider(SVector2 tl, SVector2 br)
{
	m_pTransform = NULL;
	m_vTopLeft = tl;
	m_vBottomRigth = br;
}

void CRectCollider::InitCollider(CTransform* pTransform, SVector2 pos, SVector2 size, float scale)
{
	m_pTransform = pTransform;
	m_vTopLeft = pos;
	m_vBottomRigth = size * scale;
}

SVector2& CRectCollider::GetWorldTL()
{
	static SVector2 vTL;
	vTL = CTransform::MutipleVectorToMatrix(m_vTopLeft, m_pTransform->GetTransfrom());
	return vTL;
}

SVector2& CRectCollider::GetWorldBR()
{
	static SVector2 vBR;
	vBR = CTransform::MutipleVectorToMatrix(m_vBottomRigth, m_pTransform->GetTransfrom());
	return vBR;
}

void CRectCollider::DrawOutline(CColorBrush* pColorBrush, float stroke)
{
	SVector2 vTL = CTransform::MutipleVectorToMatrix(m_vTopLeft, m_pTransform->GetTransfrom());
	SVector2 vBR = CTransform::MutipleVectorToMatrix(m_vBottomRigth, m_pTransform->GetTransfrom());
	return CDebugHelper::DrawRect(vTL, vBR, pColorBrush, stroke);
}

bool CRectCollider::ToPoint(SVector2& pos)
{
	return CCollisionCheck::OverlapPointToAABB(pos, GetWorldTL(), GetWorldBR());
}

bool CRectCollider::ToRect(CRectCollider* pRect)
{
	return CCollisionCheck::OverlapAABBtoAABB(GetWorldTL(), GetWorldBR(), pRect->GetTopLeft(), pRect->GetBottomRight());
}

bool CRectCollider::ToBox(CBoxCollider* pBox)
{
	return CCollisionCheck::OverlapAABBtoOBB(GetWorldTL(), GetWorldBR(), pBox->GetTopLeft(), pBox->GetTopRight(), pBox->GetBottomRight(), pBox->GetBottomLeft());
}

bool CRectCollider::ToCircle(CCircleCollider* pCircle)
{
	return CCollisionCheck::OverlapCircleToAABB(pCircle->GetWorldPos(), pCircle->GetRadius(), GetWorldTL(), GetWorldBR());
}

CBoxCollider::CBoxCollider(SVector2 tl, SVector2 br)
{
	m_pTransform = NULL;
	m_vTopLeft = tl;
	m_vBottomRigth = br;
	m_vTopRight = SVector2(m_vTopLeft.x, m_vBottomRigth.y);
	m_vBottomRigth = SVector2(m_vBottomRigth.x, m_vTopLeft.y);
}

void CBoxCollider::InitCollider(CTransform* pTransform, SVector2 pos, SVector2 size, float scale)
{
	m_pTransform = pTransform;
	m_vTopLeft = pos;
	m_vBottomRigth = size * scale;
	m_vTopRight = SVector2(m_vTopLeft.x, m_vBottomRigth.y);
	m_vBottomLeft = SVector2(m_vBottomRigth.x, m_vTopLeft.y);
}

SVector2& CBoxCollider::GetWorldTL()
{
	static SVector2 vTL;
	vTL = CTransform::MutipleVectorToMatrix(m_vTopLeft, m_pTransform->GetTransfrom());
	return vTL;
}

SVector2& CBoxCollider::GetWorldTR()
{
	static SVector2 vTR;
	vTR = CTransform::MutipleVectorToMatrix(m_vTopRight, m_pTransform->GetTransfrom());
	return vTR;
}

SVector2& CBoxCollider::GetWorldBL()
{
	static SVector2 vBL;
	vBL = CTransform::MutipleVectorToMatrix(m_vBottomLeft, m_pTransform->GetTransfrom());
	return vBL;
}

SVector2& CBoxCollider::GetWorldBR()
{
	static SVector2 vBR;
	vBR = CTransform::MutipleVectorToMatrix(m_vBottomRigth, m_pTransform->GetTransfrom());
	return vBR;
}

void CBoxCollider::DrawOutline(CColorBrush* pColorBrush, float stroke)
{
	/*SVector2 vTL = GetWorldTL();
	SVector2 vTR = GetWorldTR();
	SVector2 vBL = GetWorldBL();
	SVector2 vBR = GetWorldBR();*/

	SVector2 vTL = GetTopLeft();
	SVector2 vTR = GetTopRight();
	SVector2 vBL = GetBottomLeft();
	SVector2 vBR = GetBottomRight();

	CDebugHelper::DrawLine(vTL, vTR, pColorBrush, stroke);
	CDebugHelper::DrawLine(vTR, vBR, pColorBrush, stroke);
	CDebugHelper::DrawLine(vBR, vBL, pColorBrush, stroke);
	CDebugHelper::DrawLine(vBL, vTL, pColorBrush, stroke);
}

bool CBoxCollider::ToPoint(SVector2& pos)
{
	//return CCollisionCheck::OverlapPointToOBB(pos, GetTopLeft(), GetTopRight(), GetBottomRight(), GetBottomLeft());
	return CCollisionCheck::OverlapPointToOBB(pos, GetWorldTL(), GetWorldTR(), GetWorldBL(), GetWorldBR());
}

bool CBoxCollider::ToRect(CRectCollider* pRect)
{
	//return CCollisionCheck::OverlapAABBtoOBB(pRect->GetTopLeft(), pRect->GetBottomRight(), GetTopLeft(), GetTopRight(), GetBottomRight(), GetBottomLeft());
	return CCollisionCheck::OverlapAABBtoOBB(pRect->GetWorldTL(), pRect->GetWorldBR(), GetWorldTL(), GetWorldTR(), GetWorldBR(), GetWorldBL());
}

bool CBoxCollider::ToBox(CBoxCollider* pBox)
{
	//return CCollisionCheck::OverlapOBBtoOBB(GetTopLeft(), GetTopRight(), GetBottomRight(), GetBottomLeft(), pBox->GetTopLeft(), pBox->GetTopRight(), pBox->GetBottomRight(), pBox->GetBottomLeft());
	return CCollisionCheck::OverlapOBBtoOBB(GetWorldTL(), GetWorldTR(), GetWorldBR(), GetWorldBL(), pBox->GetWorldTL(), pBox->GetWorldTR(), pBox->GetWorldBR(), pBox->GetWorldBL());
}

bool CBoxCollider::ToCircle(CCircleCollider* pCircle)
{
	//return CCollisionCheck::OverlapCircleToOBB(pCircle->GetPos(), pCircle->GetRadius(), GetTopLeft(), GetTopRight(), GetBottomRight(), GetBottomLeft());
	return CCollisionCheck::OverlapCircleToOBB(pCircle->GetWorldPos(), pCircle->GetRadius(), GetWorldTL(), GetWorldTR(), GetWorldBR(), GetWorldBL());
}