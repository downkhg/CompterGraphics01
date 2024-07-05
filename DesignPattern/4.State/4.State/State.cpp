#include <iostream>
//객체의 선언을 해서 대상의 참조자를 활용할수 있음.
//그러나 각 함수의 정의시에 각 객체가 선언되지않아, 컴파일 오류가 발생함.

//클래스의 선언: 해당 클래스가 뒤에 정의된다.
//선언만 된 클래스는 참조자나 포인터만 생성가능하다.
class StateTow;
class StateThree;

//클래스의 선언: 함수의 형태를 알수 있는 정보
//클래스의 선언만 있는 경우, 구체적으로 함수를 호출하여 사용할때 컴파일오류가발생한다.
class State
{
public:
	virtual void GoNext(Context* context) = 0;
};

class StateTow : public State
{
public:
	void GoNext(Context* context);
};

class StateOne : public State
{
public:
	void GoNext(Context* context);
};

class StateThree : public State
{
public:
	void GoNext(Context* context);
};

class Context
{
	State* m_pState;
public:
	void SetState(State* state);
	void GoNext();
};

//클래스멤버함수의 정의: 선언된 클래스의 멤버함수를 정의하여 사용 가능하도록 한다.
//정의될때 실제 선언된 함수가 존제 해야 사용가능하다.
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


void Context::SetState(State* state)
{
	m_pState = state;
}

void Context::GoNext()
{
	m_pState->GoNext(this);
}