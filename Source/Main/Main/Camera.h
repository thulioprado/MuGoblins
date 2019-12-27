#pragma once

class CCamera
{
public:
	CCamera();
	~CCamera();

public:
	void Clip();
	void Restore();
	void Zoom(int Direction);

public:
	float Distance;
};

extern CCamera Camera;