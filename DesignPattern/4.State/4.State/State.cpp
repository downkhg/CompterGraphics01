#include "State.h"
#include <iostream>

using namespace std;

//��ü�� ������ �ؼ� ����� �����ڸ� Ȱ���Ҽ� ����.
//�׷��� �� �Լ��� ���ǽÿ� �� ��ü�� ��������ʾ�, ������ ������ �߻���.

//Ŭ��������Լ��� ����: ����� Ŭ������ ����Լ��� �����Ͽ� ��� �����ϵ��� �Ѵ�.
//���ǵɶ� ���� ����� �Լ��� ���� �ؾ� ��밡���ϴ�.
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