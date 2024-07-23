#include "GlobalFunc.h"

// 2를 밑으로 하는 숫자 n의 로그값을 구한다.
int	Log2( int n )
{
	for( int i = 1 ; i < 64 ; i++ )
	{
		n = n >> 1;
		if( n == 1 ) return i;
	}

	return 1;
}

// 2^n값을 구한다
int	Pow2( int n )
{
	int val = 1;
	val = val << n;
	return val;
}

// pt가 rc안에 포함되는지 검사한다.(PtInRect()라는 API함수는 음수(-)값 처리를 못했다.)
BOOL IsInRect( RECT* rc, POINT pt )
{
	if( ( rc->left <= pt.x ) && ( pt.x <= rc->right ) &&
		( rc->bottom <= pt.y ) && ( pt.y <= rc->top ) )
		return TRUE;

	return FALSE;
}
