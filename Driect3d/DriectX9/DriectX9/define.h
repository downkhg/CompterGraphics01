#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <d3d9.h>
#include <d3dx9.h>
#include "GlobalFunc.h"

#ifndef MAINBODY
#define DECLARE extern
#else
#define DECLARE
#endif

#ifndef DEL
	#define DEL(p)		{ if(p) { delete (p); (p) = NULL; } }
#endif

#ifndef DELS
	#define DELS(p)		{ if(p) { delete[] (p); (p) = NULL; } }
#endif

#ifndef REL
	#define REL(p)		{ if(p) { (p)->Release(); (p) = NULL; } }
#endif

#define IS_IN_RANGE(value,r0,r1) (( ((r0) <= (value)) && ((value) <= (r1)) ) ? 1 : 0)

#define _USE_INDEX16

struct TERRAINVERTEX
{
	enum _FVF { FVF=(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1) };
	D3DXVECTOR3	p;
	D3DXVECTOR3	n;
	D3DXVECTOR2	t;
};

struct TRIINDEX
{
#ifdef _USE_INDEX16
	WORD _0, _1, _2;
#else
	DWORD	_0, _1, _2;
#endif
};

#endif // _DEFINE_H_ 