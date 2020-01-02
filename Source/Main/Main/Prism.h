#pragma once

class CPrism
{
public:
	CPrism();
	~CPrism();

public:
	bool GetEffect(FloatColor* Color, FloatColor* Prism, int Index, int Speed);
	void GetNameEffect(BYTE* RGB, struct PrismInfo* Info);
	void GetValue(FloatColor* Prism, BYTE Red, BYTE Green, BYTE Blue);
	float InterpolateValue(float Start, float End, float Timer);

private:
	bool Equals(float A, float B);
	bool InterpolateColors(FloatColor* Color, float Timer, float R1, float G1, float B1, float R2, float G2, float B2);
};

extern CPrism Prism;