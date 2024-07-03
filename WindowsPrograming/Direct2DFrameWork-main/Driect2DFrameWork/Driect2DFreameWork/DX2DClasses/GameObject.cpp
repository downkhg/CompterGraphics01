#include "GameObject.h"
#include "Image.h"
#include "Vector2.h"
#include "DebugHelper.h"

using namespace DX2DClasses;


CTransform::CTransform()
{
	matTransform = D2D1::Matrix3x2F::Identity();
}

void CTransform::SetAsixPoint(SVector2& asix)
{
	sAsixPoint = D2D1::Point2F(asix.x, asix.x);
}

void CTransform::SetTRS(const SVector2& pos, const float angle, const SVector2& size)
{
	D2D1::Matrix3x2F matT = D2D1::Matrix3x2F::Translation(pos.x, pos.y);
	D2D1::Matrix3x2F matR = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(pos.x + sAsixPoint.x, pos.y + sAsixPoint.y));
	D2D1::Matrix3x2F matS = D2D1::Matrix3x2F::Scale(size.x, size.y);

	matTransform = matR  * matT * matS;
}

void CTransform::SetTransrate(const SVector2& pos)
{
	matTransform = D2D1::Matrix3x2F::Translation(pos.x, pos.y);
}
void CTransform::SetTransrate(float x, float y)
{
	matTransform = D2D1::Matrix3x2F::Translation(x, y);
}
void CTransform::SetRotate(const float angle)
{
	matTransform = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(sAsixPoint.x, sAsixPoint.y));
}
void CTransform::SetScale(const SVector2& size)
{
	matTransform = D2D1::Matrix3x2F::Scale(size.x, size.y);
}

void CTransform::Transrate(const SVector2& pos)
{
	D2D1::Matrix3x2F matT = D2D1::Matrix3x2F::Translation(pos.x, pos.y);
	matTransform = matTransform * matT;
}
void CTransform::Rotate(const float angle)
{
	D2D1::Matrix3x2F matR = D2D1::Matrix3x2F::Rotation(angle, D2D1::Point2F(sAsixPoint.x, sAsixPoint.y));
	matTransform = matR * matTransform;
}
void CTransform::Scale(const SVector2& size)
{
	D2D1::Matrix3x2F matS = D2D1::Matrix3x2F::Scale(size.x, size.y);
	matTransform = matTransform * matS;
}

D2D1::Matrix3x2F& CTransform::GetTransfrom()
{
	return matTransform;
}

D2D1::Matrix3x2F* CTransform::GetTransfromPtr()
{
	return &matTransform;
}

SVector2 CTransform::GetTransrate()
{
	return SVector2(matTransform.dx, matTransform.dy);
}

D2D1::Matrix3x2F CTransform::GetRotate()
{
	D2D1::Matrix3x2F matRot = D2D1::Matrix3x2F::Identity();
	matRot.m11 = matTransform.m11;
	matRot.m11 = matTransform.m12;
	matRot.m21 = matTransform.m21;
	matRot.m21 = matTransform.m22;
	return matRot;
}

SVector2 CTransform::GetScale()
{
	return SVector2(matTransform.m11, matTransform.m22);
}

SVector2 CTransform::MutipleVectorToMatrix(SVector2& vec, D2D1::Matrix3x2F& mat)
{
	D2D1_POINT_2F sPos = vec.ToPoint();
	sPos = sPos * mat;
	return SVector2(sPos);
}

SVector2 CTransform::MutipleVectorToMatrix(SVector2& vec, D2D1::Matrix3x2F* pMat)
{
	D2D1_POINT_2F sPos = vec.ToPoint();
	sPos = sPos * (*pMat);
	return SVector2(sPos);
}

CAnimator2D::CAnimator2D(int MaxSize)
{
	m_nMaxSize = MaxSize;
}

void CAnimator2D::SetFrame(int idx)
{
	m_nAnimIdx = idx;
}
void CAnimator2D::UpdateFrame()
{
	if (m_nAnimIdx < m_nMaxSize - 1)
		m_nAnimIdx++;
	else
		m_nAnimIdx = 0;
}
void CAnimator2D::DrawImage(CImage* pImage, CTransform& transform)
{
	pImage->DrawBitmap(transform.GetTransfrom(), m_nAnimIdx);
}

void CAnimator2D::SetMaxSize(int size)
{
	m_nMaxSize = size;
}
int CAnimator2D::GetAnimIndex()
{
	return m_nAnimIdx;
}

CTransform& CGameObject::GetTransform()
{
	return m_cTransform;
}
CTransform* CGameObject::GetTransformPtr()
{
	return& m_cTransform;
}

void CGameObject::SetImage(CImage* img)
{
	m_pImage = img;
}
CImage* CGameObject::GetImage()
{
	return m_pImage;
}

CGameObject::CGameObject()
{
	m_pAnimator = NULL;
}

void CGameObject::Initialize(CImage* img, bool anim, bool active)
{
	m_pAnimator = NULL;
	m_pImage = img;
	if (anim)
		m_pAnimator = new CAnimator2D(m_pImage->GetAnimationCount());
	m_isActive = active;
}
void CGameObject::Release()
{
	//CDebugHelper::LogConsole("%s:%d", typeid((*this)).name(), this);
	if (m_pAnimator)
	{
		delete m_pAnimator;
		m_pAnimator = NULL;
	}
}
void CGameObject::Update()
{
	if (m_isActive)
		m_pAnimator->UpdateFrame();
}
void CGameObject::Draw()
{
	if(m_isActive)
		m_pAnimator->DrawImage(m_pImage, m_cTransform);
}
