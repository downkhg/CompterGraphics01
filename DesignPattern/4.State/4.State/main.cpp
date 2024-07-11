#include "State.h"
#include <stdlib.h> //메모리 동적할당 헤더
#include <crtdbg.h> //메모리 누수 탐지 헤더

void main()
{
	_CrtSetBreakAlloc(170); //메모리 누수시 번호를 넣으면 할당하는 위치에 브레이크 포인트를 건다.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //메모리 누수 검사 

	Context cContext;
	//기본적으로 외부에서 객체를 할당받는것이 좋은 설계방법은 아니다.
	//cContext.SetState(new StateOne);
	cContext.GoNext();
	cContext.GoNext();
	cContext.GoNext();
}