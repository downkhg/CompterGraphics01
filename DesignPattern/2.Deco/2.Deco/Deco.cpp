#include <iostream>
using namespace std;
class Cookie
{
	int m_nFlour;
	int m_nSugar;
public:
	Cookie(int flour, int sugar)
	{
		cout << "Cookie:" << this << endl;
		m_nFlour = flour;
		m_nSugar = sugar;
	}
	~Cookie()
	{
		cout << "~Cookie:" << this << endl;
	}
};
class MilkCookie : public Cookie
{
	int m_nMilk;
public:
	MilkCookie(int flour, int sugar, int milk) :Cookie(flour, sugar)
	{
		cout << "MilkCookie:" << this << endl;
		m_nMilk = milk;
	}
	~MilkCookie()
	{
		cout << "~MilkCookie:" << this << endl;
	}
};
class ChocoCookie : public Cookie
{
	int m_nChoco;
public:
	ChocoCookie(int flour, int sugar, int choco) :Cookie(flour, sugar)
	{
		cout << "ChocoCookie:" << this << endl;
		m_nChoco = choco;
	}
	~ChocoCookie()
	{
		cout << "~ChocoCookie:" << this << endl;
	}
};
class AmondChocoCookie : public ChocoCookie
{
	int m_nAmond;
public:
	AmondChocoCookie(int flour, int sugar, int choco, int amond) :ChocoCookie(flour, sugar, choco)
	{
		cout << "AmondChocoCookie:" << this << endl;
		m_nAmond = amond;
	}
	~AmondChocoCookie()
	{
		cout << "~AmondChocoCookie:" << this << endl;
	}
};
int main()
{
	Cookie cCookie(100, 20);
	//MilkCookie cMilkCookie[10]{ MilkCookie(100,20,10), };
	//쿠키틀을 10개 준비한다.
	MilkCookie** pMilkCookies = new MilkCookie * [10];
	//밀크쿠키의 제료를 준비하여 한개씩 틀에 채워넣는다.
	for (int i = 0; i < 10; i++)
	{
		cout << i << ":";
		pMilkCookies[i] = new MilkCookie(100, 20, 20);
		//delete pMilkCookies[i];
	}
	delete[] pMilkCookies;

	AmondChocoCookie cAmondChcoCookie(100, 20, 50, 10);
}