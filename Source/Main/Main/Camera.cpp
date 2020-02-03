#include "Library.h"
#include "Camera.h"
#include "Message.h"

CCamera::CCamera() : Distance(0.f)
{
}

CCamera::~CCamera()
{
}

void CCamera::Clip()
{
	auto Multiplier = (pCameraZoom / 35.f) * 2.2f;
	auto Absolute = abs(pCameraPosZ - 150.f) * 3.f;

	Multiplier += (pResolution >= 4) ? 0.8f : 0.0f;
	
	pCameraClipX = (1190.f + Absolute) * Multiplier;
	pCameraClipY = (2400.f + Absolute) * Multiplier;
	pCameraClipB = (1128.f + Absolute) * Multiplier;
	pCameraClipGL = (2000.f + Absolute) * Multiplier;

	/*if (pMapNumber == 30)
	{
		pCameraClipCS4 = (5000.f + Absolute) * Multiplier;
		pCameraClipCS3 = (4000.f + Absolute) * Multiplier;
	}
	else
	{
		pCameraClipCS4 = 580.f;
		pCameraClipCS3 = 1300.f;
	}*/
}

void CCamera::Restore()
{
	pCameraZoom = 35.f;

	this->Clip();
}

void CCamera::Zoom(int Direction)
{
	if (pState == STATE_PLAYING)
	{
		if (Direction > 0)
		{
			if (pCameraZoom > 19.f)
			{
				pCameraZoom -= 2.f;
			}

			if (pCameraZoom < 19.f)
			{
				pCameraZoom = 19.f;
			}
		}
		else if (Direction < 0)
		{
			float MaxZoom = (pMapNumber == 30) ? 47.f : 59.f;

			if (pCameraZoom < MaxZoom)
			{
				pCameraZoom += 2.f;
			}

			if (pCameraZoom > MaxZoom)
			{
				pCameraZoom = MaxZoom;
			}
		}

		this->Distance = pCameraZoom * 10.f;
		
		this->Clip();
	}
}

CCamera Camera;