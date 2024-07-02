/*##################################
스레드를 이용한 WinAPI데모(수업용)
파일명: WinAppDemo.cpp
작성자: 김홍규 (downkhg@gmail.com)
마지막수정날짜: 2020.12.14
버전: 1.2 (메세지 주소값 출력 및 문제 단계별화)
###################################*/

#include <iostream>
#include <Windows.h>
#include <process.h>
#include <string>

using namespace std;

enum WM_MSG { CREATE, COMMOND, PAINT, DESTROY, MAX };
string strMSG[MAX] = { "CREATE","COMMOND","PAINT","DESTROY" };

bool g_bLoop = true;

//arg를 통해 외부의 데이터값을 받을수있다.
unsigned int WINAPI WndProc(void* arg)
{
	cout << "arg:" << arg << endl;
	int* pData = (int*)arg;

	while (g_bLoop)
	{
		switch (*pData)
		{
		case CREATE:
			cout << "초기화" << endl;
			*pData = COMMOND;
			break;
		case COMMOND:
			cout << "명령을 입력하세요." << endl;
			for (int i = 0; i < MAX; i++)
				cout << i << ":" << strMSG[i];
			cout << endl;
			break;
		case PAINT:
			cout << "화면에 그립니다." << endl;
			break;
		case DESTROY:
			cout << "프로그램을 종료합니다." << endl;
			g_bLoop = false;
			break;
		default:
			break;
		}
		Sleep(2000);
	}
	return 0;
}

//입력을 받으면서 화면에 메세지에 필요한 출력을 처리하는 프로그램.
//큐를 활용하여 메세지를 큐에 쌓고, 쓰레드에서 큐에서 데이터를 1개씩꺼내서 처리하는 프로그램으로 만들기.
//1. 큐를 전역변수로 만들어서 처리하기.
//2. 큐를 main의 지역변수로 만들어서 동일한 과정이 가능하도록 처리하기.
int main()
{
	HANDLE hThread = NULL;
	DWORD dwThreadID = NULL;

	int nMSG = CREATE;
	cout << "Msg:" << &nMSG << endl;
	//프로세스: 프로그램의 가장 기본적인 처리를 당담하는 흐름단위(메인루프), 큰흐름단위.
	//스레드: 프로세스 내부에 처리를 하는 흐름단위(반복문을 동시에 처리가능), 큰흐름 하위의 작은 흐름.
	//스레드는 프로그램 내부에서 처리하는 내용을 변경할수 있어야하므로, 
	//그 프로그램 내의 함수를 호출하여 사용한다.
	//콜백함수: 프로세스내에서 호출하지않고, 외부에서 호출하도록 하는 함수.
	hThread = (HANDLE)_beginthreadex(NULL, 0,
		WndProc,
		(void*)&nMSG, 0,
		(unsigned int*)dwThreadID);

	while (g_bLoop)
	{
		scanf("%d", &nMSG);
	}

	return 0;
}