#include <iostream>

using namespace std;

//싱글톤: 객체가 한개 이상 생성되지않는 클래스
class CSingleObject
{
	int m_nData;
//public :
	CSingleObject()
	{
		cout << "CSingleObject["<<this<<"]:"<< m_nData << endl;
	}
public:
	CSingleObject GetInstance()
	{
		static CSingleObject cSingleObject;
		return cSingleObject;
	}

	~CSingleObject()
	{
		cout << "~CSingleObject[" << this << "]:" << m_nData << endl;
	}
	void Display()
	{
		cout << typeid(*this).name() << ":Display(" << m_nData<< ")" << endl;
	}
};

void main()
{
	////다음과 같이 객체가 생성가능하다면 이 객체를 싱글톤이라 부를 수 없다.
	//CSingleObject cSingeObject; //객체를 정적할당한다.
	//CSingleObject arrSingleObject[2];
	//객체의 포인터만 선언했으므로, 객체가 생성되지않았다.
	CSingleObject* pSingeObject = NULL; 
	CSingleObject* pArrSingleObject[2] = {};

	pSingeObject = &pSingeObject->GetInstance();

	pSingeObject->Display(); //객체(인스턴스)가 아직 생성되지않았으므로, 멤버인 data에 접근 할 수없다. 
}