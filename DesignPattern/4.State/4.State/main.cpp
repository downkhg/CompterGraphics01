#include "State.h"
#include <stdlib.h> //�޸� �����Ҵ� ���
#include <crtdbg.h> //�޸� ���� Ž�� ���

void main()
{
	_CrtSetBreakAlloc(170); //�޸� ������ ��ȣ�� ������ �Ҵ��ϴ� ��ġ�� �극��ũ ����Ʈ�� �Ǵ�.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); //�޸� ���� �˻� 

	Context cContext;
	//�⺻������ �ܺο��� ��ü�� �Ҵ�޴°��� ���� �������� �ƴϴ�.
	//cContext.SetState(new StateOne);
	cContext.GoNext();
	cContext.GoNext();
	cContext.GoNext();
}