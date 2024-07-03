#include "CollisionCheck.h"
#include "Vector2.h"
#include "ColorBrushPalettet.h"
#include "DebugHelper.h"
#include "UtilityDefine.h"
#include <vector>

using namespace std;
using namespace DX2DClasses;
//https://gamedev.stackexchange.com/questions/25397/obb-vs-obb-collision-detection
//������ �븻�� �� ���� ������ �����ϰ� ���� ���� ū���� ���� ���� ���Ѵ�. ���⼭ ������ ������ ���̷� ����ȴ�.
void SATtest(const SVector2& axis, const SVector2 ptSet[], int size, float& minAlong, float& maxAlong, int& minIdx, int& maxIdx)
{
	minAlong = HUGE, maxAlong = -HUGE;
	for (int i = 0; i < size; i++)
	{
		// just dot it to get the min/max along this axis.
		float dotVal = SVector2::Dot(ptSet[i], axis);
		if (dotVal < minAlong) 
		{ 
			minAlong = dotVal; 
			minIdx = i; 
		}
		if (dotVal > maxAlong) 
		{ 
			maxAlong = dotVal; 
			maxIdx = i; 
		}
	}
}
inline bool isBetweenOrdered(float val, float lowerBound, float upperBound) {
	return lowerBound <= val && val <= upperBound;
}
//�� �ٸ� ������ �ּ�ũ�Ⱑ �ٸ������� �ּҰ� �ִ밪���̿� �ִٸ� �浹�Ѱ����� �����Ѵ�.
bool overlaps(float min1, float max1, float min2, float max2)
{
	return isBetweenOrdered(min2, min1, max1) || isBetweenOrdered(min1, min2, max2);
}
// Shape1 and Shape2 must be CONVEX HULLS
bool intersects(SVector2 shapeA_Corners[],int a_size, SVector2 shapeB_Corners[], int b_size)
{
	// Get the normals for one of the shapes,
	for (int i = 0; i < a_size; i++)
	{
		//������ �븻�� �����Ƿ� ������ ����� z���� ������ �̿��Ͽ� �븻�� ���Ѵ�.
		SVector2 vLine;
		if(i < a_size-1)//1-0,2-1,3-2
			vLine = shapeA_Corners[i+1] - shapeA_Corners[i];//1-0, 2-1, 3-2, 0 - 3
		else //3 - 0
			vLine = shapeA_Corners[i] - shapeA_Corners[0];
		SVector2 vNormal = SVector2::CrossZ(vLine).Normalize();
		//�������� ������ �ּ� �ִ밪�� ���Ѵ�.
		float shape1Min, shape1Max, shape2Min, shape2Max;
		int minIdxA, maxIdxA, minIdxB, maxIdxB;
		SATtest(vNormal, shapeA_Corners,a_size, shape1Min, shape1Max, minIdxA, maxIdxA);
		SATtest(vNormal, shapeB_Corners,b_size, shape2Min, shape2Max, minIdxB, maxIdxB);
		//�� ��缱�п� 1���� ��ġ�����ʴ� ��� �浹�����ʴ°����� �����Ѵ�.
		if (!overlaps(shape1Min, shape1Max, shape2Min, shape2Max))
		{
			return false; // NO INTERSECTION
		}

		// otherwise, go on with the next test
	}

	// TEST SHAPE2.normals as well

	// if overlap occurred in ALL AXES, then they do intersect
	return true;
}


bool CCollisionCheck::OverlapPointToLine(SVector2& vPos, SVector2& vStart, SVector2& vEnd, int& check)
{
#ifdef DEBUG_OVERLAP
	CColorBrush* pRedBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::E_BRUSH_COLOR::RED);
	CColorBrush* pYellowBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::E_BRUSH_COLOR::YELLOW);
	CColorBrush* pGreenBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::E_BRUSH_COLOR::GREEN);
	CColorBrush* pWhiteBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::E_BRUSH_COLOR::WHITE);
	CColorBrush* pBlackBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::E_BRUSH_COLOR::BLACK);
#endif
	SVector2 vDist = vEnd - vStart;
	SVector2 vLineCrossZ = SVector2::CrossZ(vDist);//���(����)�� ����
	SVector2 vPosToStart = vPos - vStart; //������ġ�� ���������� ���̰Ÿ����ϱ�
#ifdef DEBUG_OVERLAP
	SVector2 vNorEnd = vStart + vLineCrossZ * vDist.Magnitude();
	CDebugHelper::DrawLine(vStart, vEnd, pRedBrush);
	CDebugHelper::DrawLine(vStart, vNorEnd, pWhiteBrush);
#endif
	//���۰� ������ ���Ϳ� ���� �븻�� ������ ���Ͽ� ������ ��ġ�� ���Ѵ�.
	float fDot = SVector2::Dot(vLineCrossZ, vPosToStart);
	if (fDot == 0)
	{
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawLine(vStart, vPos, pRedBrush);
#endif
		check = E_LINE_CHECK::ON;
		return true;
	}
	else if (fDot > 0)
	{
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawLine(vStart, vPos, pGreenBrush);
#endif
		check = E_LINE_CHECK::UP;
	}
	else
	{
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawLine(vStart, vPos, pYellowBrush);
#endif
		check = E_LINE_CHECK::DOWN;
	}

	return false;
}

bool CCollisionCheck::OverlapPointToCircle(SVector2& vPos, SVector2& vCenter, const float fRad)
{
#ifdef DEBUG_OVERLAP
	CColorBrush* pRedBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::E_BRUSH_COLOR::RED);
	CColorBrush* pBlackBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::E_BRUSH_COLOR::BLACK);
#endif
	SVector2 vDist = vCenter - vPos;
	float fDist = vDist.Magnitude();

	if (fDist < fRad)
	{
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawCircle(vPos, fRad, pRedBrush);
		CDebugHelper::DrawCircle(vPos, 1, pRedBrush);
#endif
		return true;
	}
	else
	{
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawCircle(vPos, fRad, pBlackBrush);
		CDebugHelper::DrawCircle(vPos, 1, pBlackBrush);
#endif
		return false;
	}
}

bool CCollisionCheck::OverlapPointToAABB(SVector2& vPos, SVector2& vTL, SVector2& vBR)
{
#ifdef DEBUG_OVERLAP
	CColorBrush* pRedBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::E_BRUSH_COLOR::RED);
	CColorBrush* pBlackBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::E_BRUSH_COLOR::BLACK);
#endif
	if ((vTL.x < vPos.x && vPos.x < vBR.x) &&
		(vTL.y < vPos.y && vPos.y < vBR.y))
	{
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawCircle(vPos, 1, pRedBrush);
		CDebugHelper::DrawRect(vTL, vBR, pRedBrush);
#endif
		return true;
	}
	else
	{
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawCircle(vPos, 1, pBlackBrush);
		CDebugHelper::DrawRect(vTL, vBR, pBlackBrush);
#endif
		return false;
	}
}

bool CCollisionCheck::OverlapPointToOBB(SVector2& vPos, SVector2& vTL, SVector2& vTR, SVector2& vBR, SVector2& vBL)
{
	SVector2 vTLtoTR = vTR - vTL;
	SVector2 vTRtoBR = vBR - vTR;
	SVector2 vBRtoBL = vBL - vBR;
	SVector2 vBLtoTL = vTL - vBL;

	SVector2 vTLtoPos = vPos - vTL;
	SVector2 vTRtoPos = vPos - vTR;
	SVector2 vBRtoPos = vPos - vBR;
	SVector2 vBLtoPos = vPos - vBL;

	float fDotTL = SVector2::Dot(vBLtoTL.Normalize(), vTLtoPos.Normalize());
	float fDotTR = SVector2::Dot(vTLtoTR.Normalize(), vTRtoPos.Normalize());
	float fDotBR = SVector2::Dot(vTRtoBR.Normalize(), vBRtoPos.Normalize());
	float fDotBL = SVector2::Dot(vBRtoBL.Normalize(), vBLtoPos.Normalize());
	//cos    1   0  -1 
	//angle  0  90 180
	if (fDotTL < 0 && fDotTR < 0 && fDotBR < 0 && fDotBL < 0)
	{
#ifdef DEBUG_OVERLAP		
		CColorBrush* pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::YELLOW);
		CDebugHelper::DrawLine(vPos, vTL, pColorBrush);
		CDebugHelper::DrawLine(vPos, vTR, pColorBrush);
		CDebugHelper::DrawLine(vPos, vBR, pColorBrush);
		CDebugHelper::DrawLine(vPos, vBL, pColorBrush);
		pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::RED);
		CDebugHelper::DrawCircle(vPos, 1, pColorBrush);
		SVector2 rects[] = { vTL, vTR, vBR, vBL };
		CDebugHelper::DrawRect(vTL, vBR, pColorBrush);
#endif //DEBUG_OVERLAP
		return true;
	}
	else
	{
#ifdef DEBUG_OVERLAP		
		CColorBrush* pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::YELLOW);
		CDebugHelper::DrawLine(vPos, vTL, pColorBrush);
		CDebugHelper::DrawLine(vPos, vTR, pColorBrush);
		CDebugHelper::DrawLine(vPos, vBR, pColorBrush);
		CDebugHelper::DrawLine(vPos, vBL, pColorBrush);
		pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::BLACK);
		CDebugHelper::DrawCircle(vPos, 1, pColorBrush);
		SVector2 rects[] = { vTL, vTR, vBR, vBL };
		CDebugHelper::DrawRect(vTL, vBR, pColorBrush);
#endif //DEBUG_OVERLAP
	}

	return false;
}

bool CCollisionCheck::OverlapLineToAABB(SVector2& vStart, SVector2& vEnd, SVector2& vTL, SVector2& vBR)
{
	return false;
}

bool CCollisionCheck::OverlapLineToOBB(SVector2& vStart, SVector2& vEnd, SVector2& vTL, SVector2& vTR,  SVector2& vBR, SVector2& vBL)
{
	return false;
}

bool CCollisionCheck::OverlapCircleToLine(SVector2& vPos, float fRadius, SVector2& vStart, SVector2& vEnd)
{
#ifdef DEBUG_OVERLAP	
	CColorBrush* pRedBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::RED);
	CColorBrush* pBlackBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::BLACK);
	CDebugHelper::DrawLine(vStart, vEnd, pBlackBrush);
	CDebugHelper::DrawCircle(vPos, fRadius, pBlackBrush);
#endif
	//p1 = start, p2 = end , cp = pos 
	//  POINT cp_p1 = {p1.x - cp.x, p1.y - cp.y};   // p1-cp�� ������������ �ű� ��
	SVector2 vPosToStart = vStart - vPos;   // p1-cp�� ������������ �ű� ��
	SVector2 vStartToEnd = vEnd - vStart;
	//p2.x = p2.x - p1.x;               // p2-p1�� ������������ �ű� ��
	//p2.y = p2.y - p1.y;

	float dec_cp_p1 = atan2(vPosToStart.y, vPosToStart.x);  // cp_p1�� ����
	float dec_p2 = atan2(vStartToEnd.y, vStartToEnd.x);       // p2�� ����

	// ���� �߽ɿ��� ������ ������ �Ÿ�.
	//float D = sqrt(cp_p1.x * cp_p1.x + cp_p1.y * cp_p1.y) * sin(dec_p2 - dec_cp_p1);
	float D = sqrt(vPosToStart.x * vPosToStart.x + vPosToStart.y * vPosToStart.y) * sin(dec_p2 - dec_cp_p1);

	if (D < 0)
		D = -D;

	// cp-p1�� ������������ �ű� ��
	//POINT cp_p12 = { cp.x - p1.x, cp.y - p1.y };
	SVector2 vStartToPos = vPos - vStart;

	// cp_p12�� 90�� ȸ���Ѱ�
	//POINT cp_right = { -cp_p12.y, cp_p12.x };
	SVector2 vStartToPosRight = SVector2(-vStartToPos.y, vStartToPos.x);

	if (fRadius > D)
	{
		// p2�� cp_p12�� ������(�������̱ⶫ��)
		//if (p2.x * cp_right.y - p2.y * cp_right.x > 0)
		if (SVector2::Cross(vStartToEnd, vStartToPosRight) > 0)
		{
#ifdef DEBUG_OVERLAP	
			CDebugHelper::DrawLine(vStart, vEnd, pRedBrush);
			CDebugHelper::DrawCircle(vPos, fRadius, pRedBrush);
#endif
			return true;
		}
		else
			return FALSE;
	}
	else
	{
		return FALSE;
	}

	return false;
}

bool CCollisionCheck::OverlapCircleToCircle(SVector2& vCenterA, const float fRadA, SVector2& vCenterB, const float fRadB)
{
#ifdef DEBUG_OVERLAP	
	CColorBrush* pRedBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::RED);
	CColorBrush* pBlackBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::BLACK);
#endif
	SVector2 vDist = vCenterB - vCenterA;
	float fDist = vDist.Magnitude();

	if (fDist < (fRadA + fRadB))
	{
#ifdef DEBUG_OVERLAP	
		CDebugHelper::DrawCircle(vCenterA, fRadA, pRedBrush);
		CDebugHelper::DrawCircle(vCenterA, fRadA, pRedBrush);
#endif
		return true;
	}
	else
	{
#ifdef DEBUG_OVERLAP	
		CDebugHelper::DrawCircle(vCenterB, fRadB, pBlackBrush);
		CDebugHelper::DrawCircle(vCenterB, fRadB, pBlackBrush);
#endif
		return false;
	}
}

bool CCollisionCheck::OverlapCircleToAABB(SVector2& vPos, float fRadius, SVector2& vTL, SVector2& vBR)
{
	bool bCheck = false;
	SVector2 vTR = SVector2::RectTR(vTL, vBR);
	SVector2 vBL = SVector2::RectBL(vTL, vBR);
	SVector2 vTLtoTR = vTR - vTL;
	SVector2 vTRtoBR = vBR - vTR;
	SVector2 vBRtoBL = vBL - vBR;
	SVector2 vBLtoTL = vTL - vBL;

	SVector2 vTLtoPos = vPos - vTL;
	SVector2 vTRtoPos = vPos - vTR;
	SVector2 vBRtoPos = vPos - vBR;
	SVector2 vBLtoPos = vPos - vBL;

	float fTLtoPosDist = vTLtoPos.Magnitude();
	float fTRtoPosDist = vTRtoPos.Magnitude();
	float fBRtoPosDist = vBRtoPos.Magnitude();
	float fBLtoPosDist = vBLtoPos.Magnitude();

	float fDotTL = SVector2::Dot(vBLtoTL.Normalize(), vTLtoPos.Normalize());
	float fDotTR = SVector2::Dot(vTLtoTR.Normalize(), vTRtoPos.Normalize());
	float fDotBR = SVector2::Dot(vTRtoBR.Normalize(), vBRtoPos.Normalize());
	float fDotBL = SVector2::Dot(vBRtoBL.Normalize(), vBLtoPos.Normalize());

	//cos    1   0  -1 
	//angle  0  90 180
	//cos(t)�� 0���� ������ �а��̴�. �����鿡 �а��̸� �浹�̵Ȱ��̴�. 
	//if (fDotTL < 0 && fDotTR < 0 && fDotBR < 0 && fDotBL < 0)
	if (fDotTL < 0 && fDotTR < 0 && fDotBR < 0 && fDotBL < 0)
	{
#ifdef DEBUG_OVERLAP		
		CColorBrush* pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::YELLOW);
		CDebugHelper::DrawLine(vPos, vTL, pColorBrush);
		CDebugHelper::DrawLine(vPos, vTR, pColorBrush);
		CDebugHelper::DrawLine(vPos, vBR, pColorBrush);
		CDebugHelper::DrawLine(vPos, vBL, pColorBrush);
		pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::RED);
		CDebugHelper::DrawCircle(vPos, fRadius, pColorBrush);
		SVector2 rects[] = { vTL, vTR, vBR, vBL };
		CDebugHelper::DrawPolygon(rects, 4, pColorBrush);
#endif //DEBUG_OVERLAP
		bCheck = true;
	}
	else
	{
		//�浹�� �����ʴ��� �浹�鿡 �Ÿ��� ���������� ������� �浹�ȴ�.
		if (fTLtoPosDist < fRadius || fTRtoPosDist < fRadius ||
			fBRtoPosDist < fRadius || fBLtoPosDist < fRadius)
		{
#ifdef DEBUG_OVERLAP
			CColorBrush* pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::RED);
			CDebugHelper::DrawLine(vPos, vTL, pColorBrush);
			CDebugHelper::DrawLine(vPos, vTR, pColorBrush);
			CDebugHelper::DrawLine(vPos, vBR, pColorBrush);
			CDebugHelper::DrawLine(vPos, vBL, pColorBrush);
			pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::YELLOW);
			CDebugHelper::DrawCircle(vPos, fRadius, pColorBrush);
			SVector2 rects[] = { vTL, vTR, vBR, vBL };
			CDebugHelper::DrawPolygon(rects, 4, pColorBrush);
#endif // DEBUG_OVERLAP
			bCheck = true;
		}
	}

#ifdef DEBUG_OVERLAP
	if (bCheck == false)
	{
		CColorBrush* pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::YELLOW);
		CDebugHelper::DrawLine(vPos, vTL, pColorBrush);
		CDebugHelper::DrawLine(vPos, vTR, pColorBrush);
		CDebugHelper::DrawLine(vPos, vBR, pColorBrush);
		CDebugHelper::DrawLine(vPos, vBL, pColorBrush);
		pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::BLACK);
		CDebugHelper::DrawCircle(vPos, fRadius, pColorBrush);
		SVector2 rects[] = { vTL, vTR, vBR, vBL };
		CDebugHelper::DrawPolygon(rects, 4, pColorBrush);
	}
#endif // DEBUG_OVERLAP
	return bCheck;
}

bool CCollisionCheck::OverlapCircleToOBB(SVector2& vPos, float fRadius, SVector2& vTL, SVector2& vTR, SVector2& vBR, SVector2& vBL)
{
#ifdef DEBUG_OVERLAP	
	CColorBrush* pRedBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::RED);
	CColorBrush* pYellowBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::YELLOW);
	CColorBrush* pBlackBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::BLACK);
	CColorBrush* pGreenBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::GREEN);
	SVector2 rects[] = { vTL, vTR, vBR, vBL };
#endif
	bool bCheck = false;
	SVector2 vTLtoTR = vTR - vTL;
	SVector2 vTRtoBR = vBR - vTR;
	SVector2 vBRtoBL = vBL - vBR;
	SVector2 vBLtoTL = vTL - vBL;
	//������ ������ �̿��Ͽ� ������ ����� ���� ���������� ������ ������ ��� ������ ������ �F���� �����߻���.
	//������ ������ ���а��� �浹üũ�� Ȱ���ϸ� �̰��� �浹üũ�� �����ϹǷ� ���� ���ʿ����.
	/*SVector2 vTLtoTRisCenter = vTL + (vTLtoTR.Normalize() * vTLtoTR.Magnitude() * 0.5f);
	SVector2 vTRtoBRisCenter = vTR + (vTRtoBR.Normalize() * vTRtoBR.Magnitude() * 0.5f);
	SVector2 vBRtoBLisCenter = vBR + (vBRtoBL.Normalize() * vBRtoBL.Magnitude() * 0.5f);
	SVector2 vBLtoTLisCenter = vBL + (vBLtoTL.Normalize() * vBLtoTL.Magnitude() * 0.5f);*/

	SVector2 vTLtoPos = vPos - vTL;
	SVector2 vTRtoPos = vPos - vTR;
	SVector2 vBRtoPos = vPos - vBR;
	SVector2 vBLtoPos = vPos - vBL;

	/*SVector2 vTLCenterToPos = vPos - vTLtoTRisCenter;
	SVector2 vTRCenterToPos = vPos - vTRtoBRisCenter;
	SVector2 vBRCenterToPos = vPos - vBRtoBLisCenter;
	SVector2 vBLCenterToPos = vPos - vBLtoTLisCenter;*/

	float fTLtoPosDist = vTLtoPos.Magnitude();
	float fTRtoPosDist = vTRtoPos.Magnitude();
	float fBRtoPosDist = vBRtoPos.Magnitude();
	float fBLtoPosDist = vBLtoPos.Magnitude();

	/*float fTLCenterToDist = vTLCenterToPos.Magnitude();
	float fTRCenterToDist = vTRCenterToPos.Magnitude();
	float fBRCenterToDist = vBRCenterToPos.Magnitude();
	float fBLCenterToDist = vBLCenterToPos.Magnitude();*/

	float fDotTL = SVector2::Dot(vBLtoTL.Normalize(), vTLtoPos.Normalize());
	float fDotTR = SVector2::Dot(vTLtoTR.Normalize(), vTRtoPos.Normalize());
	float fDotBR = SVector2::Dot(vTRtoBR.Normalize(), vBRtoPos.Normalize());
	float fDotBL = SVector2::Dot(vBRtoBL.Normalize(), vBLtoPos.Normalize());

	/*float fDotTLCenter = SVector2::Dot(vBLtoTL.Normalize(), vTLCenterToPos.Normalize());
	float fDotTRCenter = SVector2::Dot(vTLtoTR.Normalize(), vTRCenterToPos.Normalize());
	float fDotBRCenter = SVector2::Dot(vTRtoBR.Normalize(), vBRCenterToPos.Normalize());
	float fDotBLCenter = SVector2::Dot(vBRtoBL.Normalize(), vBLCenterToPos.Normalize());*/

	//������ ������ ���� �������� ���� ���(�����浹üũ ���� ��üũ)
	float fDotTLTR = SVector2::Dot(vTLtoPos.Normalize(), vTRtoPos.Normalize());
	float fDotTRBR = SVector2::Dot(vTRtoPos.Normalize(), vBRtoPos.Normalize());
	float fDotBRBL = SVector2::Dot(vBRtoPos.Normalize(), vBLtoPos.Normalize());
	float fDotBLTL = SVector2::Dot(vBLtoPos.Normalize(), vTLtoPos.Normalize());

	//cos    1   0  -1 
	//angle  0  90 180
	//cos(t)�� 0���� ������ �а��̴�. �����鿡 �а��̸� �浹�̵Ȱ��̴�. 
	if (fDotTL < 0 && fDotTR < 0 && fDotBR < 0 && fDotBL < 0){ bCheck = true; }
#ifdef DEBUG_OVERLAP
	/*CDebugHelper::DrawLine(vPos, vTLtoTRisCenter, pBlackBrush);
	CDebugHelper::DrawLine(vPos, vTRtoBRisCenter, pBlackBrush);
	CDebugHelper::DrawLine(vPos, vBRtoBLisCenter, pBlackBrush);
	CDebugHelper::DrawLine(vPos, vBLtoTLisCenter, pBlackBrush);*/
	CDebugHelper::DrawLine(vPos, vTL, pBlackBrush);
	CDebugHelper::DrawLine(vPos, vTR, pBlackBrush);
	CDebugHelper::DrawLine(vPos, vBR, pBlackBrush);
	CDebugHelper::DrawLine(vPos, vBL, pBlackBrush);
#endif
	////�׸𼭸��� ���� ���� �浹üũ
	//if (fTLCenterToDist <= fRadius) { CDebugHelper::DrawLine(vPos, vTLtoTRisCenter, pRedBrush);  bCheck = true; }
	//if (fTRCenterToDist <= fRadius) { CDebugHelper::DrawLine(vPos, vTRtoBRisCenter, pRedBrush);  bCheck = true; }
	//if (fBRCenterToDist <= fRadius) { CDebugHelper::DrawLine(vPos, vBRtoBLisCenter, pRedBrush);  bCheck = true; }
	//if (fBLCenterToDist <= fRadius) { CDebugHelper::DrawLine(vPos, vBLtoTLisCenter, pRedBrush);  bCheck = true;}

	//�׸�𼭸� ���� �浹üũ
	if (fTLtoPosDist <= fRadius) 
	{ 
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawLine(vPos, vTL, pRedBrush); 
#endif
		bCheck = true; 
	}
	if (fTRtoPosDist <= fRadius) 
	{ 
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawLine(vPos, vTR, pRedBrush); 
#endif
		bCheck = true;
	}
	if (fBRtoPosDist <= fRadius) 
	{ 
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawLine(vPos, vBR, pRedBrush); 
#endif
		bCheck = true;
	}
	if (fBLtoPosDist <= fRadius) 
	{ 
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawLine(vPos, vBL, pRedBrush); 
#endif
		bCheck = true; 
	}

	//����� ������ �а��̶�� ������ �̿��Ͽ� �浹üũ�� �����Ѵ�.
	if (fDotTLTR < 0 && OverlapCircleToLine(vPos, fRadius, vTL, vTR))
	{
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawLine(vTL, vTR, pRedBrush);

		CDebugHelper::DrawLine(vPos, vTL, pRedBrush);
		CDebugHelper::DrawLine(vPos, vTR, pRedBrush);
#endif
		bCheck = true;
	}
	if (fDotTRBR < 0 && OverlapCircleToLine(vPos, fRadius, vTR, vBR))
	{
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawLine(vTR, vBR, pRedBrush);
		
		CDebugHelper::DrawLine(vPos, vTR, pRedBrush);
		CDebugHelper::DrawLine(vPos, vBR, pRedBrush);
#endif
		bCheck = true;
	}
	if (fDotBRBL < 0 && OverlapCircleToLine(vPos, fRadius, vBR, vBL))
	{
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawLine(vBR, vBL, pRedBrush);

		CDebugHelper::DrawLine(vPos, vBR, pRedBrush);
		CDebugHelper::DrawLine(vPos, vBL, pRedBrush);
#endif
		bCheck = true;
	}
	if (fDotBLTL < 0 && OverlapCircleToLine(vPos, fRadius, vBL, vTL))
	{
#ifdef DEBUG_OVERLAP
		CDebugHelper::DrawLine(vBL, vTL, pRedBrush);

		CDebugHelper::DrawLine(vPos, vBL, pRedBrush);
		CDebugHelper::DrawLine(vPos, vTL, pRedBrush);
#endif
		bCheck = true;
	}

	//if (bCheck)
	//{
	//	//�׸𼭸��� ���� ���� �浹üũ
	//	if (fTLCenterToDist > fRadius) { CDebugHelper::DrawLine(vPos, vTLtoTRisCenter, pRedBrush);  bCheck = false; }
	//	else { bCheck = true; }
	//	if (fTRCenterToDist > fRadius) { CDebugHelper::DrawLine(vPos, vTRtoBRisCenter, pRedBrush);  bCheck = false; }
	//	else { bCheck = true; }
	//	if (fBRCenterToDist > fRadius) { CDebugHelper::DrawLine(vPos, vBRtoBLisCenter, pRedBrush);  bCheck = false; }
	//	else { bCheck = true; }
	//	if (fBLCenterToDist > fRadius) { CDebugHelper::DrawLine(vPos, vBLtoTLisCenter, pRedBrush);  bCheck = false; }
	//	else { bCheck = true; }
	//	//�׸�𼭸� ���� �浹üũ
	//	if (fTLtoPosDist > fRadius) { CDebugHelper::DrawLine(vPos, vTL, pRedBrush); bCheck = false; }
	//	else { bCheck = true; }
	//	if (fTRtoPosDist > fRadius) { CDebugHelper::DrawLine(vPos, vTR, pRedBrush); bCheck = false; }
	//	else { bCheck = true; }
	//	if (fBRtoPosDist > fRadius) { CDebugHelper::DrawLine(vPos, vBR, pRedBrush); bCheck = false; }
	//	else { bCheck = true; }
	//	if (fBLtoPosDist > fRadius) { CDebugHelper::DrawLine(vPos, vBL, pRedBrush); bCheck = false; }
	//	else { bCheck = true; }
	//}

	/*CDebugHelper::DrawCircle(vPos, fRadius, pBlackBrush);
	CDebugHelper::DrawPolygon(rects, 4, pBlackBrush);*/

	//if (fDotTL < 0 && fDotTR < 0 && fDotBR < 0 && fDotBL < 0)
//	if ((fDotTL < 0 && fDotTR < 0 && fDotBR < 0 && fDotBL < 0))
//	{
//#ifdef DEBUG_OVERLAP		
//		CDebugHelper::DrawLine(vPos, vTL, pGreenBrush);
//		CDebugHelper::DrawLine(vPos, vTR, pGreenBrush);
//		CDebugHelper::DrawLine(vPos, vBR, pGreenBrush);
//		CDebugHelper::DrawLine(vPos, vBL, pGreenBrush);
//#endif //DEBUG_OVERLAP
//		bCheck = true;
//	}
//	else
//	{
//		//�浹�� �����ʴ��� �浹�鿡 �Ÿ��� ���������� ������� �浹�ȴ�.
//		if (fTLtoPosDist < fRadius || fTRtoPosDist < fRadius ||
//			fBRtoPosDist < fRadius || fBLtoPosDist < fRadius)
//		{
//#ifdef DEBUG_OVERLAP
//			if (fTLtoPosDist < fRadius) CDebugHelper::DrawLine(vPos, vTLtoTRisCenter, pRedBrush);
//			if (fTRtoPosDist < fRadius) CDebugHelper::DrawLine(vPos, vTRtoBRisCenter, pRedBrush);
//			if (fBRtoPosDist < fRadius) CDebugHelper::DrawLine(vPos, vBRtoBLisCenter, pRedBrush);
//			if (fBLtoPosDist < fRadius) CDebugHelper::DrawLine(vPos, vBLtoTLisCenter, pRedBrush);
//#endif // DEBUG_OVERLAP
//			bCheck = true;
//		}
//		if (fDotTLCenter < 0 && OverlapCircleToLine(vPos, fRadius, vTL, vTR))
//		{
//#ifdef DEBUG_OVERLAP
//			//CDebugHelper::DrawLine(vPos, vTLtoTRisCenter, pRedBrush);
//			CDebugHelper::DrawLine(vTL, vTR, pRedBrush);
//#endif
//			bCheck = true;
//		}
//		if (fDotTRCenter < 0 && OverlapCircleToLine(vPos, fRadius, vTR, vBR))
//		{
//#ifdef DEBUG_OVERLAP
//			//CDebugHelper::DrawLine(vPos, vTRtoBRisCenter, pRedBrush);
//			CDebugHelper::DrawLine(vTR, vBR, pRedBrush);
//#endif
//			bCheck = true;
//		}
//		if (fDotBRCenter < 0 && OverlapCircleToLine(vPos, fRadius, vBR, vBL))
//		{
//#ifdef DEBUG_OVERLAP
//			//CDebugHelper::DrawLine(vPos, vBLtoTLisCenter, pRedBrush);
//			CDebugHelper::DrawLine(vBR, vBL, pRedBrush);
//#endif
//			bCheck = true;
//		}
//		if (fDotBLCenter < 0 && OverlapCircleToLine(vPos, fRadius, vBL, vTR))
//		{
//#ifdef DEBUG_OVERLAP
//			//CDebugHelper::DrawLine(vPos, vBRtoBLisCenter, pRedBrush);
//			CDebugHelper::DrawLine(vBL, vTR, pRedBrush);
//#endif
//			bCheck = true;
//		}
//		//else
//
//	}

#ifdef DEBUG_OVERLAP
	if (bCheck)
	{
		CDebugHelper::DrawCircle(vPos, fRadius, pGreenBrush);
		CDebugHelper::DrawPolygon(rects, 4, pGreenBrush);
	}
	else
	{
		CDebugHelper::DrawCircle(vPos, fRadius, pBlackBrush);
		CDebugHelper::DrawPolygon(rects, 4, pBlackBrush);
	}
#endif

//#ifdef DEBUG_OVERLAP
//	if (bCheck == false)
//	{
//		CColorBrush* pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::YELLOW);
//		CDebugHelper::DrawLine(vPos, vTL, pColorBrush);
//		CDebugHelper::DrawLine(vPos, vTR, pColorBrush);
//		CDebugHelper::DrawLine(vPos, vBR, pColorBrush);
//		CDebugHelper::DrawLine(vPos, vBL, pColorBrush);
//		pColorBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::BLACK);
//		CDebugHelper::DrawCircle(vPos, fRadius, pColorBrush);
//		SVector2 rects[] = { vTL, vTR, vBR, vBL };
//		CDebugHelper::DrawPolygon(rects, 4, pColorBrush);
//	}
//#endif // DEBUG_OVERLAP
	return bCheck;
}

bool CCollisionCheck::OverlapAABBtoAABB(SVector2& vTL_A, SVector2& vBR_A, SVector2& vTL_B, SVector2& vBR_B)
{
#ifdef DEBUG_OVERLAP	
	CColorBrush* pRedBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::RED);
	CColorBrush* pBlackBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::BLACK);
	CDebugHelper::DrawRect(vTL_A, vBR_A, pBlackBrush);
	CDebugHelper::DrawRect(vTL_B, vBR_B, pBlackBrush);
#endif
	if (vBR_A.x < vTL_B.x || vTL_A.x > vBR_B.x) return false;
	if (vBR_A.y < vTL_B.y || vTL_A.y > vBR_B.y) return false;
#ifdef DEBUG_OVERLAP	
	CDebugHelper::DrawRect(vTL_A, vBR_A, pRedBrush);
	CDebugHelper::DrawRect(vTL_B, vBR_B, pRedBrush);
#endif
	return true;
}

//bool CCollisionCheck::OverlapAABBtoCircle(SVector2& vTL, SVector2& vBR, SVector2& vPos, float fRad)
//{
//	float fRadHalf = fRad * 0.5f;
//	SVector2 sCircleTL = vPos - SVector2(fRadHalf, fRadHalf);
//	SVector2 sCircleBR = vPos + SVector2(fRadHalf, fRadHalf);
//	if (vBR.x < sCircleTL.x || vTL.x > sCircleBR.x)
//	{
//		goto NO_CHECK;
//	}
//	if (vBR.y < sCircleTL.y || vTL.y > sCircleBR.y)
//	{
//		goto NO_CHECK;
//	}
//
//	NO_CHECK:
//	{
//		//SVector2 vTR(vBR.x, vTL.y);
//		//SVector2 vBL(vTL.x, vBR.y);
//
//		//SVector2 vTLtoPos = vPos - vTL;
//		//SVector2 vTRtoPos = vPos - vTR;
//		//SVector2 vBRtoPos = vPos - vBR;
//		//SVector2 vBLtoPos = vPos - vBL;
//
//		//float fTLtoPosDist = vTLtoPos.Magnitude();
//		//float fTRtoPosDist = vTRtoPos.Magnitude();
//		//float fBRtoPosDist = vBRtoPos.Magnitude();
//		//float fBLtoPosDist = vBLtoPos.Magnitude();
//		////�浹�� �����ʴ��� �浹�鿡 �Ÿ��� ���������� ������� �浹�ȴ�.
//		//if (fvTLtoPosDist < fRad || fTRtoPosDist < fRad ||
//		//	fBRtoPosDist < fRad || fBLtoPosDist < fRad)
//		//	return true;
//
//		return false;
//	}
//
//	return true;
//}

int ArrayMinIdx(float arr[], int size)
{
	int nMinIdx = -1;
	float fMin = 999999999;
	for (int i = 0; i < 4; i++)
	{
		if (arr[i] < fMin)
		{
			fMin = arr[i];
			nMinIdx = i;
		}
	}
	return nMinIdx;
}

bool CheckOBB(SVector2 vPos, SVector2 vCenter, SVector2 vBoxCenter, SVector2 vTL, SVector2 vTR, SVector2 vBR, SVector2 vBL)
{
	float arrDist[4];
	arrDist[0] = SVector2::Distance(vPos, vTL);
	arrDist[1] = SVector2::Distance(vPos, vTR);
	arrDist[2] = SVector2::Distance(vPos, vBR);
	arrDist[3] = SVector2::Distance(vPos, vBL);

	int nMinIdx = ArrayMinIdx(arrDist, 4);
	SVector2 vResult;
	switch (nMinIdx)
	{
	case 0: vResult = vTL; break;
	case 1: vResult = vTR; break;
	case 2: vResult = vBR; break;
	case 3: vResult = vBL; break;
	}

	float fCenterBtoTL = SVector2::Distance(vPos, vCenter);
	float fCenterAtoResult = SVector2::Distance(vResult, vBoxCenter);

	float fCenterAB = SVector2::Distance(vCenter, vBoxCenter);

	if (fCenterAB < fCenterBtoTL + fCenterAtoResult)
		return true;
	else
		return false;
}

bool CCollisionCheck::OverlapAABBtoOBB(SVector2& vTL_A, SVector2& vBR_A, SVector2& vTL_B, SVector2& vTR_B, SVector2& vBR_B, SVector2& vBL_B)
{
	SVector2 vTR_A = SVector2::RectTR(vTL_A, vBR_A);
	SVector2 vBL_A = SVector2::RectBL(vTL_A, vBR_A);
	//������ �˼������� AABB�� ���� ������ �ѵ��� �ٵǵ� ����������.
	return OverlapOBBtoOBB(vTL_A, vTR_A, vBR_A, vBL_A, vTL_B, vTR_B, vBR_B, vBL_B);;
}

//bool CCollisionCheck::OverlapOBBtoOBB(SVector2& vTL_A, SVector2& vTR_A, SVector2& vBR_A, SVector2& vBL_A, SVector2& vTL_B, SVector2& vTR_B, SVector2& vBR_B, SVector2& vBL_B)
//{
//#ifdef DEBUG_OVERLAP	
//	CColorBrush* pRedBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::RED);
//	CColorBrush* pYellowBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::YELLOW);
//	CColorBrush* pBlackBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::BLACK);
//	CColorBrush* pGreenBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::GREEN);
//#endif
//	SVector2 vCenterA = SVector2::RectCenter(vTL_A, vBR_A);
//	SVector2 vCenterB = SVector2::RectCenter(vTL_B, vBR_B);
//	float arrDist[16];
//	arrDist[0] = SVector2::Distance(vTL_A, vTL_B);
//	arrDist[1] = SVector2::Distance(vTL_A, vTR_B);
//	arrDist[2] = SVector2::Distance(vTL_A, vBR_B);
//	arrDist[3] = SVector2::Distance(vTL_A, vBL_B);
//
//	arrDist[4] = SVector2::Distance(vTR_A, vTL_B);
//	arrDist[5] = SVector2::Distance(vTR_A, vTR_B);
//	arrDist[6] = SVector2::Distance(vTR_A, vBR_B);
//	arrDist[7] = SVector2::Distance(vTR_A, vBL_B);
//
//	arrDist[8] = SVector2::Distance(vBR_A, vTL_B);
//	arrDist[9] = SVector2::Distance(vBR_A, vTR_B);
//	arrDist[10] = SVector2::Distance(vBR_A, vBR_B);
//	arrDist[11] = SVector2::Distance(vBR_A, vBL_B);
//
//	arrDist[12] = SVector2::Distance(vBL_A, vTL_B);
//	arrDist[13] = SVector2::Distance(vBL_A, vTR_B);
//	arrDist[14] = SVector2::Distance(vBL_A, vBR_B);
//	arrDist[15] = SVector2::Distance(vBL_A, vBL_B);
//
//	int nMinIdx = ArrayMinIdx(arrDist, 16);
//	SVector2 vResultA, vResultB;
//	SVector2 vStartA, vEndA;
//	SVector2 vStartB, vEndB;
//	switch (nMinIdx)
//	{
//	case 0: vResultA = vTL_A; vResultB = vTL_B; vStartA = vTL_A; vEndA = vTR_A; vStartB = vTL_B; vEndB = vTR_B;  break;
//	case 1: vResultA = vTL_A; vResultB = vTR_B; vStartA = vTL_A; vEndA = vTR_A; vStartB = vTR_B; vEndB = vBR_B; break;
//	case 2: vResultA = vTL_A; vResultB = vBR_B; vStartA = vTL_A; vEndA = vTR_A; vStartB = vBR_B; vEndB = vBL_B; break;
//	case 3: vResultA = vTL_A; vResultB = vBL_B; vStartA = vTL_A; vEndA = vTR_A; vStartB = vBL_B; vEndB = vTL_B; break;
//
//	case 4: vResultA = vTR_A; vResultB = vTL_B; vStartA = vTR_A; vEndA = vBR_A; vStartB = vTL_B; vEndB = vTR_B; break;
//	case 5: vResultA = vTR_A; vResultB = vTR_B; vStartA = vTR_A; vEndA = vBR_A; vStartB = vTR_B; vEndB = vBR_B; break;
//	case 6: vResultA = vTR_A; vResultB = vBR_B; vStartA = vTR_A; vEndA = vBR_A; vStartB = vBR_B; vEndB = vBL_B; break;
//	case 7: vResultA = vTR_A; vResultB = vBL_B; vStartA = vTR_A; vEndA = vBR_A; vStartB = vBL_B; vEndB = vTL_B; break;
//
//	case 8: vResultA = vBR_A; vResultB = vTL_B; vStartA = vBR_A; vEndA = vBL_A; vStartB = vTL_B; vEndB = vTR_B; break;
//	case 9: vResultA = vBR_A; vResultB = vTR_B; vStartA = vBR_A; vEndA = vBL_A; vStartB = vTR_B; vEndB = vBR_B; break;
//	case 10: vResultA = vBR_A; vResultB = vBR_B; vStartA = vBR_A; vEndA = vBL_A; vStartB = vBR_B; vEndB = vBL_B; break;
//	case 11: vResultA = vBR_A; vResultB = vBL_B; vStartA = vBR_A; vEndA = vBL_A; vStartB = vBL_B; vEndB = vTL_B; break;
//
//	case 12: vResultA = vBL_A; vResultB = vTL_B; vStartA = vBL_A; vEndA = vTL_A; vStartB = vTL_B; vEndB = vTR_B; break;
//	case 13: vResultA = vBL_A; vResultB = vTR_B; vStartA = vBL_A; vEndA = vTL_A; vStartB = vTR_B; vEndB = vBR_B; break;
//	case 14: vResultA = vBL_A; vResultB = vBR_B; vStartA = vBL_A; vEndA = vTL_A; vStartB = vBR_B; vEndB = vBL_B; break;
//	case 15: vResultA = vBL_A; vResultB = vBL_B; vStartA = vBL_A; vEndA = vTL_A; vStartB = vBL_B; vEndB = vTL_B; break;
//	}
//
//	float fCenterToResultA = SVector2::Distance(vResultA, vCenterA);
//	float fCenterToResultB = SVector2::Distance(vResultB, vCenterB);
//
//	float fCenterAB = SVector2::Distance(vCenterA, vCenterB);
//
//	CDebugHelper::DrawLine(vCenterA, vCenterB, pYellowBrush);
//
////	if (OverlapCircleToOBB(vCenterA, fCenterToResultA, vTL_B, vTR_B, vBR_B, vBL_B))
////	{
////#ifdef DEBUG_OVERLAP	
////		CDebugHelper::DrawLine(vResultA, vResultB, pGreenBrush);
////#endif
////		return true;
////	}
////	else
////	{
////#ifdef DEBUG_OVERLAP	
////		CDebugHelper::DrawLine(vResultA, vResultB, pRedBrush);
////#endif
////		return false;
////	}
//
//
//	if (fCenterAB < fCenterToResultA + fCenterToResultB)
//	{
//#ifdef DEBUG_OVERLAP	
//		CDebugHelper::DrawLine(vResultA, vResultB, pGreenBrush);
//		return true;	
//#endif
//	}
//	else
//	{
//#ifdef DEBUG_OVERLAP	
//		CDebugHelper::DrawLine(vResultA, vResultB, pRedBrush);
//#endif
//	}
//
//	return false;
//}

bool CCollisionCheck::OverlapOBBtoOBB(SVector2& vTL_A, SVector2& vTR_A, SVector2& vBR_A, SVector2& vBL_A, SVector2& vTL_B, SVector2& vTR_B, SVector2& vBR_B, SVector2& vBL_B)
{
#ifdef DEBUG_OVERLAP	
	CColorBrush* pRedBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::RED);
	CColorBrush* pYellowBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::YELLOW);
	CColorBrush* pBlackBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::BLACK);
	CColorBrush* pGreenBrush = CColorBrushPalettet::GetInstance()->GetBrushClass(CColorBrushPalettet::GREEN);
#endif
	//�ϴ� �κ� �浹üũ�� ������ ���� �����غ���.
	int nSizeA = 4;
	int nSizeB = 4;
	SVector2 boxAPoints[4]; 
	boxAPoints[0] = vTL_A; boxAPoints[1] = vTR_A; boxAPoints[2] = vBR_A; boxAPoints[3] = vBL_A;
	SVector2 boxBPoints[4]; 
	boxBPoints[0] = vTL_B; boxBPoints[1] = vTR_B; boxBPoints[2] = vBR_B; boxBPoints[3] = vBL_B;
	//guswo
	for (int i = 0; i < nSizeA; i++)
	{
		//������ �븻�� �����Ƿ� ������ ����� z���� ������ �̿��Ͽ� �븻�� ���Ѵ�.
		SVector2 vLine;
		if (i < nSizeA - 1)//1-0,2-1,3-2
			vLine = boxAPoints[i + 1] - boxAPoints[i];//1-0, 2-1, 3-2, 0 - 3
		else //3 - 0
			vLine = boxAPoints[0] - boxAPoints[i];
		SVector2 vNormal = SVector2::CrossZ(vLine).Normalize();
		//�������� ������ �ּ� �ִ밪�� ���Ѵ�.
		float boxAMin, boxAMax, boxBMin, boxBMax;
		int minIdxA = 0, maxIdxA = 0, minIdxB = 0, maxIdxB = 0;
		SATtest(vNormal, boxAPoints, nSizeA, boxAMin, boxAMax, minIdxA, maxIdxA);
		SATtest(vNormal, boxBPoints, nSizeB, boxBMin, boxBMax, minIdxB, maxIdxB);
#ifdef DEBUG_OVERLAP
		SVector2 vBoxA = SVector2::RectCenter(vTL_A, vBR_A);
		SVector2 vBoxB = SVector2::RectCenter(vTL_B, vBR_B);
		//SVector2 vNormalEnd = vBoxA + vNormal;
		CDebugHelper::DrawLine(vBoxA, boxAPoints[minIdxA], pGreenBrush);
		CDebugHelper::DrawLine(vBoxA, boxAPoints[maxIdxA], pYellowBrush);

		CDebugHelper::DrawLine(vBoxB, boxBPoints[minIdxB], pGreenBrush);
		CDebugHelper::DrawLine(vBoxB, boxBPoints[maxIdxB], pYellowBrush);
		//CDebugHelper::DrawLine(vBoxA, vNormalEnd, pBlackBrush);
#endif
		//�� ��缱�п� 1���� ��ġ�����ʴ� ��� �浹�����ʴ°����� �����Ѵ�.
		if (!overlaps(boxAMin, boxBMax, boxAMin, boxBMax))
		{
			return false; // NO INTERSECTION
		}
		// otherwise, go on with the next test
	}
#ifdef DEBUG_OVERLAP
	//SVector2 vNormalEnd = vBoxA + vNormal;
	CDebugHelper::DrawLine(vTL_A, vTR_A, pRedBrush);
	CDebugHelper::DrawLine(vTR_A, vBR_A, pRedBrush);
	CDebugHelper::DrawLine(vBR_A, vBL_A, pRedBrush);
	CDebugHelper::DrawLine(vBL_A, vTL_A, pRedBrush);
	//CDebugHelper::DrawLine(vBoxA, vNormalEnd, pBlackBrush);
#endif
	// TEST SHAPE2.normals as well

	// if overlap occurred in ALL AXES, then they do intersect
	return true;
}

