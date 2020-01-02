#include "Library.h"
#include "Prism.h"
#include "Player.h"

CPrism::CPrism()
{
}

CPrism::~CPrism()
{
}

bool CPrism::GetEffect(FloatColor* Color, FloatColor* Prism, int Index, int Speed)
{
	int Rand = (Index * 18765 + 44162) % RAND_MAX;
	int Rate = 505 + Speed;

	if (Rate < 10)
	{
		Rate = 10;
	}

	if (Rate > 1000)
	{
		Rate = 1000;
	}

	float Timer = (1.0f + sin((TickCount + (Rand % Rate)) / ((float)(Rate) + (float)(Rand % 50)))) * 0.5f;

	if ((Prism[0].Red != 0.f || Prism[0].Green != 0.f || Prism[0].Blue != 0.f) &&
		(Prism[1].Red != 0.f || Prism[1].Green != 0.f || Prism[1].Blue != 0.f))
	{
		this->InterpolateColors(Color, Timer, Prism[0].Red, Prism[0].Green, Prism[0].Blue, Prism[1].Red, Prism[1].Green, Prism[1].Blue);
	}
	else if (Prism[0].Red != 0.f || Prism[0].Green != 0.f || Prism[0].Blue != 0.f)
	{
		Color->Red = Prism[0].Red;
		Color->Green = Prism[0].Green;
		Color->Blue = Prism[0].Blue;
	}
	else if (Prism[1].Red != 0.f || Prism[1].Green != 0.f || Prism[1].Blue != 0.f)
	{
		Color->Red = Prism[1].Red;
		Color->Green = Prism[1].Green;
		Color->Blue = Prism[1].Blue;
	}
	else
	{
		return false;
	}

	return true;
}

void CPrism::GetNameEffect(BYTE* RGB, PrismInfo* Info)
{
	float Timer = (1.0f + sin(TickCount / 500.0f)) * 0.5f;
	RGB[0] = (BYTE)(this->InterpolateValue((float)(Info->Color[0].Red), (float)(Info->Color[1].Red), Timer));
	RGB[1] = (BYTE)(this->InterpolateValue((float)(Info->Color[0].Green), (float)(Info->Color[1].Green), Timer));
	RGB[2] = (BYTE)(this->InterpolateValue((float)(Info->Color[0].Blue), (float)(Info->Color[1].Blue), Timer));
}

void CPrism::GetValue(FloatColor* Prism, BYTE Red, BYTE Green, BYTE Blue)
{
	BYTE RedValue[] = {GET_NIBBLE_X(Red), GET_NIBBLE_Y(Red)};
	BYTE GreenValue[] = {GET_NIBBLE_X(Green), GET_NIBBLE_Y(Green)};
	BYTE BlueValue[] = {GET_NIBBLE_X(Blue), GET_NIBBLE_Y(Blue)};

	Prism[0].Red = float(RedValue[0] * 0x11) / 255.f;
	Prism[1].Red = float(RedValue[1] * 0x11) / 255.f;
	Prism[0].Green = float(GreenValue[0] * 0x11) / 255.f;
	Prism[1].Green = float(GreenValue[1] * 0x11) / 255.f;
	Prism[0].Blue = float(BlueValue[0] * 0x11) / 255.f;
	Prism[1].Blue = float(BlueValue[1] * 0x11) / 255.f;
}

float CPrism::InterpolateValue(float Start, float End, float Timer)
{
	return Start + (End - Start) * Timer;
}

bool CPrism::Equals(float A, float B)
{
	return fabs(A - B) < 0.001f;
}

bool CPrism::InterpolateColors(FloatColor* Color, float Timer, float R1, float G1, float B1, float R2, float G2, float B2)
{
	Color->Red = this->InterpolateValue(R1, R2, Timer);
	Color->Green = this->InterpolateValue(G1, G2, Timer);
	Color->Blue = this->InterpolateValue(B1, B2, Timer);

	return Equals(Color->Red, R2) && Equals(Color->Green, G2) && Equals(Color->Blue, B2);
}

CPrism Prism;