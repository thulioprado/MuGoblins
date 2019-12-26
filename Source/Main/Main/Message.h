#pragma once

class CMessage
{
public:
	CMessage();
	~CMessage();

public:
	void Load();
	const char* Get(int Index) const;

private:
	BYTE Language;
};

extern CMessage Message;