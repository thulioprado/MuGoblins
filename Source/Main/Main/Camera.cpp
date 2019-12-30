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
			if (pCameraZoom < 59.f)
			{
				pCameraZoom += 2.f;
			}

			if (pCameraZoom > 59.f)
			{
				pCameraZoom = 59.f;
			}
		}

		this->Distance = pCameraZoom * 10.f;
		
		this->Clip();
	}
}

CCamera Camera;