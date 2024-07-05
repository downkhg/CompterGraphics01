#include <iostream>
#include <vector>
using namespace std;
//������ ����Ʈ������ ���迡���� ���� �����̳� �߻����� ���� �� ���� �ʿ�� ����.
//���� ��ӹ޾� ����� �۾��ڿ� ������ ����ϴ°��� �� �������� �ִ�.
//��ü���⹮���� ���� �ݵ�� ��ų�ʿ�� ������ �����ؾ��Ѵ�.

//������ ���� �𵨸��Ҷ��� ���������Ͽ� ����쳪.
//Ŀ����� ��ɿ� ���� �۵��ϴ°��� ����ϹǷ� ��������� ���Ҽ��ִ�.

//�����Լ�: ���������Լ��� 1���̻��� Ŭ����.
//�������ε�: �Լ��� ��� ��Ÿ�ӽñ⿡ ������
//���Լ������Ϳ� �Ҵ�� �����ϴ� �Լ��� �ּҰ��� �ٲ��.
//���θ�ü�� �ڽİ�ü(�ν��Ͻ�)�� �Ҵ��ϸ� �����Լ��� �ڽ��� ������� �۵��Ѵ�.
//���ݹ��Լ��� ����Ѵ�.
//�����Լ����̺�(v-ptr):
//��������Ʈ(C#):
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
//��üȭ: ����Ŭ������ ��ӹ޾� �ڽ�Ŭ������ ����
//������ ���������ʴ� ������ ������ �����ϴ� ������ �Ӽ��� �����ϴ� ��.
class Marin : public Unit
{
	//�������̵�: �����Լ��� ������ �����ϴ� ��.
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
	//���������Լ��� ��������������, �����ÿ� ������ �߻��Ѵ�.
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
	//�δ뿡 ����� ������ ���
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
	
	//�߻�Ŭ������ ��ü�� �����Ҽ�����. 
	//�����̶�� ������ ���� �Ҽ�����.
	//Unit unit;
	Marin marins[10];
	Medic medics[10];

	for (int i = 0; i < 10; i++)
		cCommder.AddGroup(&marins[i]);

	for (int i = 0; i < 10; i++)
		cCommder.AddGroup(&medics[i]);

	cCommder.Command(Commader::E_COMMAND::ATK, &marins[0]);
}