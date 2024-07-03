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

	class CTestScene : public ISceneManager
	{
		CColorBrushPalettet* m_pColorBrushPalettet;

		CImage* m_pPlayer;
		CImage* m_pOpossum;
		CImage* m_pEagle;
		CImage* m_pCherry;
		CImage* m_pGem;
		CImage* m_pItemEffect;
		CImage* m_pDeathEffect;

		CGameObject* m_pPlayerObject;
		CGameObject* m_pOpossumObject;
		CGameObject* m_pEagleObject;
		CGameObject* m_pCherryObject;
		CGameObject* m_pGemObject;

		CGameObject* m_pItemEffectObject;
		CGameObject* m_pDeathEffectObject;

		SVector2 m_vPos;
	public:
		CTestScene();
		~CTestScene();

		void Initialize(HWND hWnd, CDriect2DFramwork* pDX2DFramework) override; //�ʱ�ȭ
		void Release() override; //������ü ����

		void Update() override; //��������
		void Draw() override; //��ü �׸���

		void MatrixTest();
	};
}