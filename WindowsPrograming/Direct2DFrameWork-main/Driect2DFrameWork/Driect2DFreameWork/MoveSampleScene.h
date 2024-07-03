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

	class CMoveSampleScene : public ISceneManager
	{
		//색상정보가 설정된 객체 디버그도우미와 함께 사용한다.
		CColorBrushPalettet* m_pColorBrushPalettet;

		//게임이 필요한 이미지를 가져오는 객체
		CImage* m_pPlayerImage;
		CImage* m_pOpossumImage;
		CImage* m_pEagleImage;

		CImage* m_pCherryImage;
		CImage* m_pGemImage;

		CImage* m_pItemEffectImage;
		CImage* m_pDeathEffectImage;

		//실제로 로드된 이미지를 토대로 애니메이션 및 변형을 수행한다.
		//게임오브젝트는 실제복제된 객체도 포함도므로 더 많은게 일반적이다.
		CGameObject* m_pPlayerObject;
		CGameObject* m_pOpossumObject;
		CGameObject* m_pEagleObject;
		
		CGameObject* m_pCherryObject;
		CGameObject* m_pGemObject;

		CGameObject* m_pItemEffectObject;
		CGameObject* m_pDeathEffectObject;

		float m_fPlayerSpeed = 2;
		float m_fPlayerJumpHigher = 7;
		float m_fOpossumSpeed = 3;
		float m_fEangleSpeed = 5;
		
	public:
		CMoveSampleScene();
		~CMoveSampleScene();

		void Reset(); //초기위치를 설정하는 함수

		void Initialize(HWND hWnd, CDriect2DFramwork* pDX2DFramework) override; //초기화
		void Release() override; //동적객체 제거

		void Update() override; //정보변경
		void Draw() override; //객체 그리기
	};
}