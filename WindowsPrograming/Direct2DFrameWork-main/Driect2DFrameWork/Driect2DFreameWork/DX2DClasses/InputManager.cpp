#include "InputManager.h"
#include <Windows.h>

using namespace DX2DClasses;

bool CInputManager::GetAsyncKeyStatePress(int nKey)
{
	if (GetAsyncKeyState(nKey) & E_KEY_STATE::PRESS) return true;
	return false;
}

bool CInputManager::GetAsyncKeyStateRelease(int nKey)
{
	if (GetAsyncKeyState(nKey) & E_KEY_STATE::RELEASE) return true;
	return false;
}

bool CInputManager::GetAsyncKeyStateType(int nKey, E_KEY_STATE eState)
{
	if (GetAsyncKeyState(nKey) & eState) return true;
	return false;
}