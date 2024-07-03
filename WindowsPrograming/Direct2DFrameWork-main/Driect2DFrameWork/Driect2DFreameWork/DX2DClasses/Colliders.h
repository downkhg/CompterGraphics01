#pragma once
#define NULL 0
#include "Vector2.h"

namespace DX2DClasses
{
	class CCircleCollider;
	class CRectCollider;
	class CBoxCollider;
	class CPolygon;
	class CTransform;
	class CColorBrush;

	class CCollider
	{
	protected:
		CTransform* m_pTransform;
	public:
		CCollider(CTransform* pTransfrom = NULL){ m_pTransform = pTransfrom; }
		virtual void InitCollider(CTransform* pTransfrom, SVector2 pos, SVector2 size, float scale) = 0;

		virtual void DrawOutline(CColorBrush* pColorBrush, float stroke = 1) = 0;

		virtual bool ToPoint(SVector2& vec) = 0;
		virtual bool ToRect(CRectCollider* pRect) = 0;
		virtual bool ToBox(CBoxCollider* pBox) = 0;
		virtual bool ToCircle(CCircleCollider* pCircle) = 0;
	};

	class CCircleCollider : public CCollider
	{
		SVector2 m_vPos;
		float m_fRadius;
	public:
		SVector2& GetPos() { return m_vPos; }
		float GetRadius() { return m_fRadius; }
	public:
		CCircleCollider(CTransform* pTransform = NULL, SVector2 pos = SVector2(), float radius = 0);
		CCircleCollider(CTransform* pTransform, SVector2 pos, SVector2 size, float scale = 0.5f);
		void InitCollider(CTransform* pTransform, SVector2 pos, SVector2 size, float scale = 0.3f) override;
		
		SVector2& GetWorldPos();

		void DrawOutline(CColorBrush* pColorBrush, float stroke = 1) override;

		bool ToPoint(SVector2& pos) override;
		bool ToRect(CRectCollider* rect) override;
		bool ToBox(CBoxCollider* box) override;
		bool ToCircle(CCircleCollider* circle) override;
	};

	class CRectCollider : public CCollider
	{
		SVector2 m_vTopLeft;
		SVector2 m_vBottomRigth;
	public:
		SVector2& GetTopLeft() { return m_vTopLeft; };
		SVector2& GetBottomRight() { return m_vBottomRigth; };
	public:
		CRectCollider(SVector2 tl = SVector2(), SVector2 br = SVector2());
		void InitCollider(CTransform* pTransform, SVector2 pos, SVector2 size, float scale = 1) override;
		
		SVector2& GetWorldTL();
		SVector2& GetWorldBR();

		void DrawOutline(CColorBrush* pColorBrush, float stroke = 1) override;

		bool ToPoint(SVector2& vec) override;
		bool ToRect(CRectCollider* pRect) override;
		bool ToBox(CBoxCollider* pBox) override;
		bool ToCircle(CCircleCollider* pCircle) override;
	};

	class CBoxCollider : public CCollider
	{
		SVector2 m_vTopLeft;
		SVector2 m_vTopRight;
		SVector2 m_vBottomRigth;
		SVector2 m_vBottomLeft;
	public:
		SVector2& GetTopLeft() { return m_vTopLeft; };
		SVector2& GetBottomRight() { return m_vBottomRigth; };
		SVector2& GetTopRight() {  return m_vTopRight; };
		SVector2& GetBottomLeft() { return m_vBottomLeft; };
	public:
		CBoxCollider(SVector2 tl = SVector2(), SVector2 br = SVector2());
		void InitCollider(CTransform* pTransform, SVector2 pos, SVector2 size, float scale = 1) override;

		SVector2& GetWorldTL();
		SVector2& GetWorldTR();
		SVector2& GetWorldBL();
		SVector2& GetWorldBR();

		void DrawOutline(CColorBrush* pColorBrush, float stroke = 1) override;

		bool ToPoint(SVector2& vec) override;
		bool ToRect(CRectCollider* pRect) override;
		bool ToBox(CBoxCollider* pBox) override;
		bool ToCircle(CCircleCollider* pCircle) override;
	};
}
