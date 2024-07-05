#include <iostream>
//��ü�� ������ �ؼ� ����� �����ڸ� Ȱ���Ҽ� ����.
//�׷��� �� �Լ��� ���ǽÿ� �� ��ü�� ��������ʾ�, ������ ������ �߻���.

//Ŭ������ ����: �ش� Ŭ������ �ڿ� ���ǵȴ�.
//���� �� Ŭ������ �����ڳ� �����͸� ���������ϴ�.
class StateTow;
class StateThree;

//Ŭ������ ����: �Լ��� ���¸� �˼� �ִ� ����
//Ŭ������ ���� �ִ� ���, ��ü������ �Լ��� ȣ���Ͽ� ����Ҷ� �����Ͽ������߻��Ѵ�.
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

//Ŭ��������Լ��� ����: ����� Ŭ������ ����Լ��� �����Ͽ� ��� �����ϵ��� �Ѵ�.
//���ǵɶ� ���� ����� �Լ��� ���� �ؾ� ��밡���ϴ�.
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