#include "SingletonRenderTarget.h"

using namespace DX2DClasses;

ID2D1HwndRenderTarget* CSingletonRenderTarget::m_pRenderTarget = NULL;

CSingletonRenderTarget::CSingletonRenderTarget(ID2D1HwndRenderTarget* pRenderTarget)
{ 
	if (pRenderTarget != NULL) m_pRenderTarget = pRenderTarget; 
}
ID2D1HwndRenderTarget* CSingletonRenderTarget::GetRenderTarget()
{ 
	return m_pRenderTarget; 
}
void CSingletonRenderTarget::CreateRenderTarget()
{

}
void CSingletonRenderTarget::SetRenderTarget(ID2D1HwndRenderTarget* pRenderTarget)
{ 
	m_pRenderTarget = pRenderTarget; 
}