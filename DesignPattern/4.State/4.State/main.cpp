#include "State.h"

void main()
{
	Context cContext;

	cContext.SetState(new StateOne);
	cContext.GoNext();
	cContext.GoNext();
	cContext.GoNext();
}