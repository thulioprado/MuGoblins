#include "Library.h"
#include "Viewport.h"

CViewport::CViewport()
{
}

CViewport::~CViewport()
{
}

void CViewport::Clear()
{
	for (int i = 0; i < VIEWPORT_MAX; ++i)
	{
		((ViewportInfo*)(VIEWPORT_BASE + sizeof(ViewportInfo) * i))->Live = false;
	}
}

ViewportInfo* CViewport::GetMyself()
{
	return (ViewportInfo*)(pMyViewport);
}

ViewportInfo* CViewport::GetByPosition(int Position)
{
	if (Position >= VIEWPORT_MAX)
	{
		return null;
	}

	return (ViewportInfo*)(VIEWPORT_BASE + sizeof(ViewportInfo) * Position);
}

ViewportInfo* CViewport::GetByIndex(short Index)
{
	ViewportInfo* Viewport = null;

	for (int i = 0; i < VIEWPORT_MAX; ++i)
	{
		Viewport = (ViewportInfo*)(VIEWPORT_BASE + sizeof(ViewportInfo) * i);

		if (Viewport->Live && Viewport->Index == Index)
		{
			break;
		}
	}

	return Viewport;
}

ViewportInfo* CViewport::GetByName(char* Name)
{
	ViewportInfo* Viewport = null;

	for (int i = 0; i < VIEWPORT_MAX; ++i)
	{
		Viewport = (ViewportInfo*)(VIEWPORT_BASE + sizeof(ViewportInfo) * i);

		if (Viewport->Live && !strcmp(Viewport->Name, Name))
		{
			break;
		}
	}

	return Viewport;
}

CViewport Viewport;