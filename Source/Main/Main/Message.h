#pragma once

class CMessage
{
public:
	CMessage();
	~CMessage();

public:
	void Load();
	const char* Get(int Index) const;
	void Success(int Index, ...);
	void Error(int Index, ...);
	void Info(int Index, ...);
	void SetColor(int Index);

public:
	static void ShowInTab();
	static void SetColor1();
	static void SetColor2();
	static void SetColor3();
	static void SetColor4();

private:
	BYTE Language;
	char Text[256];
};

extern CMessage Message;