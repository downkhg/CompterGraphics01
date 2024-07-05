#pragma once

//복잡한 선언과 정의를 분리하여 보기가 편하게 만든다.
//cpp의 내용을 라이브러리로 빌드하여 배포하고,
//실제구현한 cpp파일을 노출하지않아 코드를 보안하는데 사용된다.

//클래스의 선언: 해당 클래스가 뒤에 정의된다.
//선언만 된 클래스는 참조자나 포인터만 생성가능하다.
class State;
class StateOne;
class StateTow;
class StateThree;
class Context;

//클래스의 선언: 함수의 형태를 알수 있는 정보
//클래스의 선언만 있는 경우, 구체적으로 함수를 호출하여 사용할때 컴파일오류가발생한다.
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