/*##################################
�����带 �̿��� WinAPI����(������)
���ϸ�: WinAppDemo_Queueing.cpp
�ۼ���: ��ȫ�� (downkhg@gmail.com)
������������¥: 2020.12.15
����: 1.0 (���������)
###################################*/

#include <iostream>
#include <Windows.h>
#include <process.h>
#include <string>
#include <queue>

using namespace std;

enum WM_MSG { CREATE, COMMOND, PAINT, DESTROY, MAX };
string strMSG[MAX] = { "CREATE","COMMOND","PAINT","DESTROY" };

bool g_bLoop = true;
queue<int> g_queMsg;

//arg�� ���� �ܺ��� �����Ͱ��� �������ִ�.
unsigned int WINAPI WndProc(void* arg)
{
	cout << "WndProc Start - arg:" << arg << endl;

	//int* pData = (int*)arg;
	int nCount = 0;
	while (g_bLoop)
	{
		cout << "Thread Loop While:" << nCount << endl;
		if (g_queMsg.empty() == false)
		{
			int nMsg = g_queMsg.front();
			cout << "Queue:" << nMsg << endl;
			switch (nMsg)
			{
			case CREATE:
				cout << "�ʱ�ȭ" << endl;
				nMsg = COMMOND;
				break;
			case COMMOND:
				cout << "����� �Է��ϼ���." << endl;
				for (int i = 0; i < MAX; i++)
					cout << i << ":" << strMSG[i];
				cout << endl;
				break;
			case PAINT:
				cout << "ȭ�鿡 �׸��ϴ�." << endl;
				break;
			case DESTROY:
				cout << "���α׷��� �����մϴ�." << endl;
				g_bLoop = false;
				break;
			default:
				break;
			}
			g_queMsg.pop();
			Sleep(2000);
			nCount++;
		}
	}
	cout << "WndProc End - arg:" << arg << endl;
	return 0;
}

//�Է��� �����鼭 ȭ�鿡 �޼����� �ʿ��� ����� ó���ϴ� ���α׷�.
//ť�� Ȱ���Ͽ� �޼����� ť�� �װ�, �����忡�� ť���� �����͸� 1���������� ó���ϴ� ���α׷����� �����.
//1. ť�� ���������� ���� ó���ϱ�.
//2. ť�� main�� ���������� ���� ������ ������ �����ϵ��� ó���ϱ�.

int main()
{
	HANDLE hThread = NULL;
	DWORD dwThreadID = NULL;
	//queue<int> queMsg;

	int nMSG = CREATE;
	cout << "Msg:" << &nMSG << endl;
	//���μ���: ���α׷��� ���� �⺻���� ó���� ����ϴ� �帧����(���η���), ū�帧����.
	//������: ���μ��� ���ο� ó���� �ϴ� �帧����(�ݺ����� ���ÿ� ó������), ū�帧 ������ ���� �帧.
	//������� ���α׷� ���ο��� ó���ϴ� ������ �����Ҽ� �־���ϹǷ�, 
	//�� ���α׷� ���� �Լ��� ȣ���Ͽ� ����Ѵ�.
	//�ݹ��Լ�: ���μ��������� ȣ�������ʰ�, �ܺο��� ȣ���ϵ��� �ϴ� �Լ�.
	hThread = (HANDLE)_beginthreadex(NULL, 0,
		WndProc,
		//(void*)&queMsg, 0,
		NULL, 0,
		(unsigned int*)dwThreadID);

	while (g_bLoop)
	{
		scanf("%d", &nMSG);
		g_queMsg.push(nMSG);
	}

	return 0;
}