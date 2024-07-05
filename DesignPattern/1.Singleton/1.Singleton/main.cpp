#include <iostream>

using namespace std;

//�̱���: ��ü�� �Ѱ� �̻� ���������ʴ� Ŭ����
class CSingleObject
{
	static CSingleObject* m_pSingleObject; //�����Ҵ簴ü(�ν��Ͻ�)�� �������踦 Ȱ���Ͽ� �����Ѵ�.
	int m_nData;
//public :
	CSingleObject()
	{
		cout << "CSingleObject["<<this<<"]:"<< m_nData << endl;
	}
	~CSingleObject()
	{
		//delete(m_pSingleObject); //��ü���ο��� ��ü�� �����Ϸ��� �ܺο��� delete�� ȣ���ؾ��ϴµ�, �Ұ����ϹǷ� ȣ���Ҽ�����.
		cout << "~CSingleObject[" << this << "]:" << m_nData << endl;
	}
public:
	////�������������� Ȱ���Ͽ� �̱����Ҽ� ����. �׷��� �ٸ� ��ü������� ���Ұ�.
	//CSingleObject& GetInstance()
	//{
	//	static CSingleObject cSingleObject;
	//	return cSingleObject;
	//}
	static CSingleObject* GetInstance()
	{
		//��ü�� �������� ȣ��Ǵ� ��������Լ������� this�� ȣ�� �� �� ����.
		//cout << "CSingleObject::GetInstance[" << this << "]:" << m_nData << endl;
		cout << "CSingleObject::GetInstance()" << endl;
		if(!m_pSingleObject)
			m_pSingleObject = new CSingleObject();

		return m_pSingleObject;
		//return new CSingleObject();//��ü�� ȣ��ɶ����� �����Ǹ�, �̱����̾ƴϸ� ���� ��ü(�ν��Ͻ�)�� ������ ������ ��ü�� å����������.
	}

	static void Release()
	{
		cout << "CSingleObject::Release()" << endl;
		if (m_pSingleObject)//���� �������� �����ϵ��� �����ϴ� ���� �����ϴ�.
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

//�����������: ������������� ��ü�� �������� �Ҵ�Ǿ��ϴµ�, 
//c���� �Լ�ȣ������ �����Ҽ��ִ� ������ �������� �ۿ� �����Ƿ� �̷������� ������ �ۼ��ؾ��Ѵ�.
CSingleObject* CSingleObject::m_pSingleObject = NULL;

void SingletonTestMain()
{
	cout << "SingletonTestMain Start!" << endl;
	////������ ���� ��ü�� ���������ϴٸ� �� ��ü�� �̱����̶� �θ� �� ����.
	//CSingleObject cSingeObject; //��ü�� �����Ҵ��Ѵ�.
	//CSingleObject arrSingleObject[2];
	//��ü�� �����͸� ���������Ƿ�, ��ü�� ���������ʾҴ�.
	CSingleObject* pSingeObject = NULL;
	CSingleObject* pArrSingleObject[2] = {};

	//CSingleObject cSingleObj; //��ü�� �����̺��̹Ƿ�, ���ϰ����� ��ü�� �����Ҽ� ����.
	//CSingleObject& refSingleObj = pSingeObject->GetInstance();
	//refSingleObj.Display();
	////������ ���� ������ ��ü�� ������, ȣ���� ������ ������ ��������� �����������⶧���� �����ϴ�.
	////�׷��� �����ش��� ���ΰ�ü�� �����Ѵٴ� ������ �����ɼ������Ƿ� ��� ����.
	//pSingeObject = pSingeObject->GetInstance();
	pSingeObject = CSingleObject::GetInstance();
	cout << "pSingeObject:" << pSingeObject << endl;
	for (int i = 0; i < 2; i++)
	{
		pArrSingleObject[i] = CSingleObject::GetInstance();
		cout << "Array[" << i << "]:" << pArrSingleObject << endl;
		//delete pArrSingleObject[i]; //�Ҵ�� ��ü�� ��ü ���ο��� ó���ϵ��� �Ҹ��ڸ� priavte�Ѵ�.
	}
	//delete pSingeObject;
	//��ü�� �Ҹ��� ��ü�ܺο����� �����ؾ��Ѵ�.
	pSingeObject->Release(); 
	CSingleObject::Release();
	//pSingeObject->Display(); //��ü(�ν��Ͻ�)�� ���� ���������ʾ����Ƿ�, ����� data�� ���� �� ������. ��Ÿ�� ������ �߻��Ѵ�.
	cout << "SingletonTestMain End!" << endl;
}

void main()
{
	SingletonTestMain();
}