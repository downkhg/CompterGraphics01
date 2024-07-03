#pragma once

namespace DX2DClasses
{
	struct SVector2;

	class CCollisionCheck
	{
	public:
		enum E_LINE_CHECK { UP, ON, DOWN };
	public:
		static bool OverlapPointToLine(SVector2& vPos, SVector2& vStart, SVector2& vEnd, int& check);
		static bool OverlapPointToCircle(SVector2& vPos, SVector2& vCenter, const float fRad);
		static bool OverlapPointToAABB(SVector2& vPos, SVector2& vTL, SVector2& br);
		static bool OverlapPointToOBB(SVector2& vPos, SVector2& vTL, SVector2& vTR, SVector2& vBR, SVector2& vBL);

		static bool OverlapLineToAABB(SVector2& vStart, SVector2& vEnd, SVector2& vTL, SVector2& vBR);
		static bool OverlapLineToOBB(SVector2& vStart, SVector2& vEnd, SVector2& vTL, SVector2& vTR, SVector2& vBR, SVector2& vBL);

		static bool OverlapCircleToLine(SVector2& vPos, float fRadius, SVector2& vStart, SVector2& vEnd);
		static bool OverlapCircleToCircle(SVector2& vCenterA, const float fRadA, SVector2& vCenterB, const float fRadB);
		static bool OverlapCircleToAABB(SVector2& vCenter, const float fRad, SVector2& vTL, SVector2& vBR);
		static bool OverlapCircleToOBB(SVector2& vCenter, const float fRad, SVector2& vTL, SVector2& vTR, SVector2& vBR, SVector2& vBL);
		
		static bool OverlapAABBtoAABB(SVector2& vTL_A, SVector2& vBR_A, SVector2& vTL_B, SVector2& vTR_B);
		static bool OverlapAABBtoOBB(SVector2& vTL_A, SVector2& vBR_A, SVector2& vTL_B, SVector2& vTR_B, SVector2& vBR_B, SVector2& vBL_B);// == OBB to OBB

		static bool OverlapOBBtoOBB(SVector2& vTL_A, SVector2& vTR_A, SVector2& vBR_A, SVector2& vBL_A, SVector2& vTL_B, SVector2& vTR_B, SVector2& vBR_B, SVector2& vBL_B);

	};
}