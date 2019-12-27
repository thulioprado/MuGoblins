#pragma once

#define WM_TRAYICON				2000

#define WM_TIMER_CLICK_LEFT		10000
#define WM_TIMER_CLICK_RIGHT	10001

class CWindow
{
public:
	CWindow();
	~CWindow();

public:
	void Attach(HINSTANCE Instance);
	void Detach();
	HWND GetHandle() const;

private:
	static HWND WINAPI		CreateWindowEx(DWORD ExStyle, LPCSTR ClassName, LPCSTR WindowName, DWORD Style, int X, int Y, int Width, int Height, HWND Parent, HMENU Menu, HINSTANCE Instance, LPVOID Parameter);
	static LONG WINAPI		ChangeDisplay(DEVMODEA* Mode, DWORD Flag);
	static SHORT WINAPI		GetKeyState(int Key);
	static void WINAPI		ReadPixels(GLint X, GLint Y, GLsizei Width, GLsizei Height, GLenum Format, GLenum Type, GLvoid* Pixels);
	static HFONT WINAPI		CreateFont(int Height, int Width, int Escapement, int Orientation, int Weight, DWORD Italic, DWORD Underline, DWORD StrikeOut, DWORD CharSet, DWORD OutPrecision, DWORD ClipPrecision, DWORD Quality, DWORD PitchAndFamily, LPCSTR FaceName);
	static LRESULT CALLBACK Procedure(HWND Window, UINT Code, WPARAM W, LPARAM L);

private:
	HINSTANCE		Instance;
	HWND			Window;
	bool			WindowMode;
	NOTIFYICONDATA	Notify;
};

extern CWindow Window;