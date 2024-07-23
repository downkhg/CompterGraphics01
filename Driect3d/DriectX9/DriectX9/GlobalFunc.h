#ifndef _GLOBALFUNC_H_
#define _GLOBALFUNC_H_

#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>

// 2를 밑으로 하는 숫자 n의 로그값을 구한다.
int	Log2( int n );

// 2^n값을 구한다
int	Pow2( int n );

// pt가 rc안에 포함되는지 검사한다.(PtInRect()라는 API함수는 음수(-)값 처리를 못했다.)
BOOL IsInRect( RECT* rc, POINT pt );

#endif // _GLOBALFUNC_H_