#pragma once

//������ ����� ���Ǹ� �и��Ͽ� ���Ⱑ ���ϰ� �����.
//cpp�� ������ ���̺귯���� �����Ͽ� �����ϰ�,
//���������� cpp������ ���������ʾ� �ڵ带 �����ϴµ� ���ȴ�.

//Ŭ������ ����: �ش� Ŭ������ �ڿ� ���ǵȴ�.
//���� �� Ŭ������ �����ڳ� �����͸� ���������ϴ�.
class State;
class StateOne;
class StateTow;
class StateThree;
class Context;

//Ŭ������ ����: �Լ��� ���¸� �˼� �ִ� ����
//Ŭ������ ���� �ִ� ���, ��ü������ �Լ��� ȣ���Ͽ� ����Ҷ� �����Ͽ������߻��Ѵ�.
class State
{
public:
	virtual void GoNext(Context* context) = 0;
};

class StateOne : public State
{
public:
	StateOne();
	~StateOne();
	void GoNext(Context* context);
};

class StateTow : public State
{
public:
	StateTow();
	~StateTow();
	void GoNext(Context* context);
};

class StateThree : public State
{
public:
	StateThree();
	~StateThree();
	void GoNext(Context* context);
};

class Context
{
	State* m_pState;
public:
	Context();
	~Context();
	void SetState(State* state);
	void GoNext();
};