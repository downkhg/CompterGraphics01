#include "State.h"
#include <iostream>

using namespace std;

//객체의 선언을 해서 대상의 참조자를 활용할수 있음.
//그러나 각 함수의 정의시에 각 객체가 선언되지않아, 컴파일 오류가 발생함.

//클래스멤버함수의 정의: 선언된 클래스의 멤버함수를 정의하여 사용 가능하도록 한다.
//정의될때 실제 선언된 함수가 존제 해야 사용가능하다.
StateOne::StateOne()
{
	cout << typeid(*this).name() << endl;
}

StateOne::~StateOne()
{
	cout <<"~"<< typeid(*this).name() << endl;
}

StateTow::StateTow()
{
	cout << typeid(*this).name() << endl;
}

StateTow::~StateTow()
{
	cout << "~" << typeid(*this).name() << endl;
}

StateThree::StateThree()
{
	cout << typeid(*this).name() << endl;
}

StateThree::~StateThree()
{
	cout << "~" << typeid(*this).name() << endl;
}

void StateThree::GoNext(Context* context)
{
	context->SetState(new StateOne);
}

void StateTow::GoNext(Context* context) 
{
	context->SetState(new StateThree);
}
;

void StateOne::GoNext(Context* context)
{
	context->SetState(new StateTow);
}

Context::Context()
{
	m_pState = new StateOne();

	cout << typeid(*this).name() << endl;
}

Context::~Context()
{
	if (m_pState)
	{
		delete m_pState;
	}

	cout << "~" << typeid(*this).name() << endl;
}

void Context::SetState(State* state)
{
	if (m_pState)
	{
		delete m_pState;
	}
	
	m_pState = state;
}

void Context::GoNext()
{
	m_pState->GoNext(this);
}