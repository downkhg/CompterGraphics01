#pragma once
#include <d2d1helper.h>


namespace DX2DClasses
{
	struct SVector2;
	class CImage;

	class CTransform
	{
		D2D1::Matrix3x2F matTransform;
		D2D1_POINT_2F sAsixPoint;
	public:
		CTransform();

		void SetAsixPoint(SVector2& asix);//회전 될 중점 설정

		void SetTRS(const SVector2& pos, const float fAngle, const SVector2& size);
		void SetTransrate(const SVector2& pos);
		void SetTransrate(float x, float y);
		void SetRotate(const float fAngle);
		void SetScale(const SVector2& size);

		void Transrate(const SVector2& pos);
		void Rotate(const float fAngle);
		void Scale(const SVector2& size);

		D2D1::Matrix3x2F& GetTransfrom();
		D2D1::Matrix3x2F* GetTransfromPtr();

		SVector2 GetTransrate();
		D2D1::Matrix3x2F GetRotate();
		SVector2 GetScale();

		static SVector2 MutipleVectorToMatrix(SVector2& vec, D2D1::Matrix3x2F& mat);
		static SVector2 MutipleVectorToMatrix(SVector2& vec, D2D1::Matrix3x2F* mat);
	};

	class CAnimator2D
	{
		int m_nAnimIdx = 0;
		int m_nMaxSize = 0;
	public:
		CAnimator2D(int MaxSize);
		
		void SetFrame(int idx = 0);
		void UpdateFrame();
		void DrawImage(CImage* pImage, CTransform& transform);

		void SetMaxSize(int size);
		int GetMaxSize();
		int GetAnimIndex();
	};

	class CGameObject
	{
		CTransform m_cTransform;
		CAnimator2D* m_pAnimator = NULL;
		CImage* m_pImage = NULL;
		bool m_isActive;
	public:
		CTransform& GetTransform();
		CTransform* GetTransformPtr();
		void SetImage(CImage* img);
		CImage* GetImage();

		void SetActive(bool use) { m_isActive = use; };
		bool GetActive() { return m_isActive; };
	public:
		CGameObject();

		void Initialize(CImage* img = NULL, bool anim = false, bool acive = true);
		void Release();
		void Update();
		void Draw();
	};
}