#pragma once

#include "DX2DClasses/SceneManager.h"
#include "DX2DClasses/Vector2.h"

namespace DX2DClasses
{
	class CRect;
	class CPolygon;
	class CTriangle;
	class CCircle;
	class CImage;
	class CColorBrush;
	class CColorBrushPalettet;
	class CGameObject;
	class CCircleCollider;
	class CRectCollider;
	class CBoxCollider;

	class CollisionTestScene : public ISceneManager
	{
		CColorBrushPalettet* m_pColorBrushPalettet;

		CImage* m_pPlayerImage;
		CImage* m_pOpossumImage;
		CImage* m_pEagleImage;

		CGameObject* m_pPlayerObject;
		CGameObject* m_pOpossumObject;
		CGameObject* m_pEagleObject;

		CBoxCollider* m_pPlayerBoxCollider;
		CRectCollider* m_pPlayerRectCollider;
		CCircleCollider* m_pPlayerCircleCollider;

		CBoxCollider* m_pOpossumBoxCollider;
		CRectCollider* m_pOpossumRectCollider;

		CCircleCollider* m_pEagleCircleCollider;

	public:
		CollisionTestScene();
		~CollisionTestScene();

		void Initialize(HWND hWnd, CDriect2DFramwork* pDX2DFramework) override; //�ʱ�ȭ
		void Release() override; //������ü ����

		void Update() override; //��������
		void Draw() override; //��ü �׸���
	public: //����� ���� �ɹ�
		bool m_isColPlayerToOpossum;
		bool m_isColPlayerToEagle;

		void CollisionCheckDraw();
		void ColliderDraw();
	};
}