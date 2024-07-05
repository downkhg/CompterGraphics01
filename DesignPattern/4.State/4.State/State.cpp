#include <iostream>
//��ü�� ������ �ؼ� ����� �����ڸ� Ȱ���Ҽ� ����.
//�׷��� �� �Լ��� ���ǽÿ� �� ��ü�� ��������ʾ�, ������ ������ �߻���.
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

