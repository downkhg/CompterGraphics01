#include <iostream>
#include <vector>
using namespace std;
//실제로 소프트웨어의 설계에서는 실제 현상이나 추상적인 것을 꼭 따를 필요는 없다.
//실제 상속받아 사용할 작업자에 편리성을 고려하는것이 더 나을수도 있다.
//객체지향문법을 따라서 반드시 지킬필요는 없음을 인지해야한다.

//다음과 같이 모델링할때는 전략자패턴에 가까우나.
//커멘더의 명령에 따라 작동하는것은 비슷하므로 옵져버라고 말할수있다.

//가상함수: 순수가상함수를 1개이상가진 클래스.
//동적바인딩: 함수의 기능 런타임시기에 결정됨
//ㄴ함수포인터에 할당된 참조하는 함수의 주소값이 바뀐다.
//ㄴ부모객체에 자식객체(인스턴스)를 할당하면 가상함수가 자식의 기능으로 작동한다.
//ㄴ콜백함수를 등록한다.
//가상함수테이블(v-ptr):
//델리게이트(C#):
class  Unit
{
public:
	Unit();
	int x;
	int y;
	virtual void Attack(Unit* target) = 0
	{
		cout << typeid(*this).name() << "Attack" << endl;
	}
	virtual void Move(int x, int y) = 0
	{
		cout << typeid(*this).name() << "Move:" << x << "," << y << endl;
	}
	virtual void SkillA(Unit* target)
	{
		cout << "SkillA" << endl;
	}
	virtual void SkillB(Unit* target)
	{
		cout << "SkillB" << endl;
	}
	virtual void SkillC(Unit* target)
	{
		cout << "SkillC" << endl;
	}
};
//실체화: 가상클래스를 상속받아 자식클래스를 생성
//실제로 존제하지않는 유닛을 실제로 존재하는 마린의 속성을 정의하는 것.
class Marin : public Unit
{
	//오버라이드: 가상함수를 실제로 구현하는 것.
	void SkillA(Unit* unit) override
	{
		cout << "Active StillPack" << endl;
	}
	void Attack(Unit* target) override
	{
		Unit::Attack(target);
	}
	void Move(int x, int y) override
	{
		Unit::Move(x, y);
	}
};

class Medic : public Unit
{
	void Attack(Unit* unit) override
	{
		Move(unit->x, unit->y);
	}
	//순수가상함수를 선언하지않으면, 생성시에 오류가 발생한다.
	void SkillA(Unit* unit) override
	{
		cout << "Active Hill" << endl;
	}

	void SkillB(Unit* unit) override
	{
		cout << "Recovery" << endl;
	}
};

class Commader
{

	vector<Unit*> group;
public:
	enum E_COMMAND { ATK, MOV };

	void AddGroup(Unit* unit)
	{
		group.push_back(unit);
	}

	bool RemvoeGroup(Unit* unit)
	{
		vector<Unit*>::iterator it;
		for (it = group.begin(); it != group.end(); it++)
		{
			if (&unit == &(*it))
				break;
		}
		if (it != group.end())
		{
			group.erase(it);
			return true;
		}
		return false;
	}
	//부대에 명령을 내리는 기능
	void Command(E_COMMAND command, Unit* target)
	{
		switch (command)
		{
		case E_COMMAND::ATK:
			for (int i = 0; i < group.size(); i++)
				group[i]->Attack(target);
			break;
		case E_COMMAND::MOV:
			for (int i = 0; i < group.size(); i++)
				group[i]->Move(10, 10);
			break;
		default:
			break;
		}
	}
};

void main()
{
	Commader cCommder;
	
	//추상클래스는 객체를 생성할수없음. 
	//유닛이라는 유닛은 존재 할수없다.
	//Unit unit;
	Marin marins[10];
	Medic medics[10];

	for (int i = 0; i < 10; i++)
		cCommder.AddGroup(&marins[i]);

	for (int i = 0; i < 10; i++)
		cCommder.AddGroup(&medics[i]);

	cCommder.Command(Commader::E_COMMAND::ATK, &marins[0]);
}