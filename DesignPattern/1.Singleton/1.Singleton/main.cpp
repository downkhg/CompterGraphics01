#include <iostream>

using namespace std;

//�̱���: ��ü�� �Ѱ� �̻� ���������ʴ� Ŭ����
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
	////������ ���� ��ü�� ���������ϴٸ� �� ��ü�� �̱����̶� �θ� �� ����.
	//CSingleObject cSingeObject; //��ü�� �����Ҵ��Ѵ�.
	//CSingleObject arrSingleObject[2];
	//��ü�� �����͸� ���������Ƿ�, ��ü�� ���������ʾҴ�.
	CSingleObject* pSingeObject = NULL; 
	CSingleObject* pArrSingleObject[2] = {};

	pSingeObject = &pSingeObject->GetInstance();

	pSingeObject->Display(); //��ü(�ν��Ͻ�)�� ���� ���������ʾ����Ƿ�, ����� data�� ���� �� ������. 
}