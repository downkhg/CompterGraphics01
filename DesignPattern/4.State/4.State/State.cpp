#include <iostream>
//객체의 선언을 해서 대상의 참조자를 활용할수 있음.
//그러나 각 함수의 정의시에 각 객체가 선언되지않아, 컴파일 오류가 발생함.
class StateTow;
class StateThree;

class State
{
	void GoNext(Context* context)
	{
		context->GoNext();
	}
};

class StateTow
{
	void GoNext(Context* context)
	{
		context->SetState(new StateThree);
	}
};

class StateOne : public State
{
	void GoNext(Context* context)
	{
		context->SetState(new StateTow);
	}
};

class StateThree
{
	void GoNext(Context* context)
	{
		context.SetState(new StateOne);
	}
};

class Context
{
	State* m_pState;
public:
	void SetState(State* state)
	{
		m_pState = state;
	}

	void GoNext()
	{
		m_pState->GoNext(this);
	}
};

