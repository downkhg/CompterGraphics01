#include <iostream>

using namespace std;

//싱글톤: 객체가 한개 이상 생성되지않는 클래스
class CSingleObject
{
	static CSingleObject* m_pSingleObject; //동적할당객체(인스턴스)를 연관관계를 활용하여 관리한다.
	int m_nData;
//public :
	CSingleObject()
	{
		cout << "CSingleObject["<<this<<"]:"<< m_nData << endl;
	}
	~CSingleObject()
	{
		//delete(m_pSingleObject); //객체내부에서 객체를 삭제하려면 외부에서 delete를 호출해야하는데, 불가능하므로 호출할수없다.
		cout << "~CSingleObject[" << this << "]:" << m_nData << endl;
	}
public:
	////정적지역변수를 활용하여 싱글톤할수 있음. 그러나 다른 객체지향언어에서 사용불가.
	//CSingleObject& GetInstance()
	//{
	//	static CSingleObject cSingleObject;
	//	return cSingleObject;
	//}
	static CSingleObject* GetInstance()
	{
		//객체가 생성전에 호출되는 정적멤버함수에서는 this를 호출 할 수 없다.
		//cout << "CSingleObject::GetInstance[" << this << "]:" << m_nData << endl;
		cout << "CSingleObject::GetInstance()" << endl;
		if(!m_pSingleObject)
			m_pSingleObject = new CSingleObject();

		return m_pSingleObject;
		//return new CSingleObject();//객체가 호출될때마다 생성되며, 싱글톤이아니며 각자 객체(인스턴스)의 참조를 생성된 객체가 책임질수없음.
	}

	static void Release()
	{
		cout << "CSingleObject::Release()" << endl;
		if (m_pSingleObject)//값이 있을때만 삭제하도록 설정하는 것이 안전하다.
		{
			delete m_pSingleObject;
			m_pSingleObject = NULL;
		}
	}

	void Display()
	{
		cout << typeid(*this).name() << ":Display(" << m_nData<< ")" << endl;
	}
};

//정적멤버변수: 정적멤버변수는 객체가 생성전에 할당되야하는데, 
//c에서 함수호출전에 생성할수있는 변수는 전역변수 밖에 없으므로 이런식으로 문법을 작성해야한다.
CSingleObject* CSingleObject::m_pSingleObject = NULL;

void SingletonTestMain()
{
	cout << "SingletonTestMain Start!" << endl;
	////다음과 같이 객체가 생성가능하다면 이 객체를 싱글톤이라 부를 수 없다.
	//CSingleObject cSingeObject; //객체를 정적할당한다.
	//CSingleObject arrSingleObject[2];
	//객체의 포인터만 선언했으므로, 객체가 생성되지않았다.
	CSingleObject* pSingeObject = NULL;
	CSingleObject* pArrSingleObject[2] = {};

	//CSingleObject cSingleObj; //객체가 프라이빗이므로, 리턴값으로 객체를 지정할수 없다.
	//CSingleObject& refSingleObj = pSingeObject->GetInstance();
	//refSingleObj.Display();
	////다음과 같은 문법은 객체가 없지만, 호출이 가능한 이유는 멤버변수에 접근한적없기때문에 가능하다.
	////그러나 가능해더라도 널인객체에 접근한다는 개념은 성립될수없으므로 사용 금지.
	//pSingeObject = pSingeObject->GetInstance();
	pSingeObject = CSingleObject::GetInstance();
	cout << "pSingeObject:" << pSingeObject << endl;
	for (int i = 0; i < 2; i++)
	{
		pArrSingleObject[i] = CSingleObject::GetInstance();
		cout << "Array[" << i << "]:" << pArrSingleObject << endl;
		//delete pArrSingleObject[i]; //할당된 객체는 객체 내부에서 처리하도록 소멸자를 priavte한다.
	}
	//delete pSingeObject;
	//객체의 소멸은 객체외부에서도 가능해야한다.
	pSingeObject->Release(); 
	CSingleObject::Release();
	//pSingeObject->Display(); //객체(인스턴스)가 아직 생성되지않았으므로, 멤버인 data에 접근 할 수없다. 런타임 오류가 발생한다.
	cout << "SingletonTestMain End!" << endl;
}

void main()
{
	SingletonTestMain();
}