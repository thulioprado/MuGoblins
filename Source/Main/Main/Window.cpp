#include "Library.h"
#include "Window.h"
#include "resource.h"
#include "Resolution.h"

auto pCreateWindowEx = CreateWindowEx;
auto pChangeDisplaySettings = ChangeDisplaySettings;
auto pGetAsyncKeyState = GetAsyncKeyState;
auto pglReadPixels = glReadPixels;
auto pCreateFont = CreateFont;

CWindow::CWindow() : Instance(null), Window(null), WindowMode(false), Notify{}
{
}

CWindow::~CWindow()
{
}

void CWindow::Attach(HINSTANCE Instance)
{
	this->Instance = Instance;
	
	//
	// Ler registro
	//
	this->WindowMode = Registry::Read<bool>("WindowMode");

	// 
	// Attach
	//
	Memory::Change<DWORD>(0x4B36EB, DWORD(&this->Procedure));

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(LPVOID&)(pCreateWindowEx), this->CreateWindowEx);
	DetourAttach(&(LPVOID&)(pChangeDisplaySettings), this->ChangeDisplay);
	DetourAttach(&(LPVOID&)(pGetAsyncKeyState), this->GetKeyState);
	DetourAttach(&(LPVOID&)(pglReadPixels), this->ReadPixels);
	DetourAttach(&(LPVOID&)(pCreateFont), this->CreateFont);
	DetourTransactionCommit();

	Resolution.Load();
}

void CWindow::Detach()
{
	//
	// Detach
	//
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(LPVOID&)(pCreateWindowEx), this->CreateWindowEx);
	DetourDetach(&(LPVOID&)(pChangeDisplaySettings), this->ChangeDisplay);
	DetourDetach(&(LPVOID&)(pGetAsyncKeyState), this->GetKeyState);
	DetourDetach(&(LPVOID&)(pglReadPixels), this->ReadPixels);
	DetourDetach(&(LPVOID&)(pCreateFont), this->CreateFont);
	DetourTransactionCommit();

	if (!IsWindowVisible(this->Window))
	{
		Shell_NotifyIcon(NIM_DELETE, &this->Notify);
	}
}

HWND CWindow::GetHandle() const
{
	return this->Window;
}

HWND WINAPI CWindow::CreateWindowEx(DWORD ExStyle, LPCSTR ClassName, LPCSTR WindowName, DWORD Style, int X, int Y, int Width, int Height, HWND Parent, HMENU Menu, HINSTANCE Instance, LPVOID Parameter)
{
	if (IS_INTRESOURCE(ClassName))
	{
		return pCreateWindowEx(ExStyle, ClassName, WindowName, Style, X, Y, Width, Height, Parent, Menu, Instance, Parameter);
	}

	if (!_stricmp(ClassName, "MU"))
	{
		if (::Window.WindowMode)
		{
			RECT rc = {0, 0, Width, Height};

			// Topmost
			ExStyle = (false ? WS_EX_TOPMOST : WS_EX_APPWINDOW);

			// Borda
			Style = (true ? WS_CAPTION | WS_BORDER | WS_SYSMENU : WS_POPUP);

			// Ajusta as dimensões da janela
			if (AdjustWindowRectEx(&rc, Style, false, ExStyle))
			{
				Width = -rc.left + rc.right;
				Height = -rc.top + rc.bottom;
			}

			//ex_style = 0;
			X = (GetSystemMetrics(SM_CXSCREEN) / 2) - (Width / 2);
			Y = (GetSystemMetrics(SM_CYSCREEN) / 2) - (Height / 2);
			//Parent = 0;
			//Menu = 0;
			//style = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
		}

		::Window.Window = pCreateWindowEx(ExStyle, ClassName, WindowName, Style, X, Y, Width, Height, Parent, Menu, Instance, Parameter);

		memset(&::Window.Notify, 0, sizeof(::Window.Notify));
		strcpy_s(::Window.Notify.szTip, WindowName);

		::Window.Notify.cbSize = sizeof(::Window.Notify);
		::Window.Notify.hWnd = ::Window.Window;
		::Window.Notify.uID = 10002;
		::Window.Notify.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		::Window.Notify.uCallbackMessage = WM_TRAYICON;
		::Window.Notify.hIcon = LoadIcon(::Window.Instance, MAKEINTRESOURCE(TRAY_ICON));
		::Window.Notify.hBalloonIcon = LoadIcon(::Window.Instance, MAKEINTRESOURCE(TRAY_ICON));

		return ::Window.Window;
	}

	return pCreateWindowEx(ExStyle, ClassName, WindowName, Style, X, Y, Width, Height, Parent, Menu, Instance, Parameter);
}

LONG WINAPI CWindow::ChangeDisplay(DEVMODEA* Mode, DWORD Flag)
{
	if (::Window.WindowMode)
	{
		return DISP_CHANGE_SUCCESSFUL;
	}

	return pChangeDisplaySettings(Mode, Flag);
}

SHORT WINAPI CWindow::GetKeyState(int Key)
{
	if (GetForegroundWindow() == ::Window.Window)
	{
		return pGetAsyncKeyState(Key);
	}

	if (!IsWindowVisible(::Window.Window))
	{
		if (pGetAsyncKeyState(VK_F12) & 0x8000)
		{
			ShowWindow(::Window.Window, SW_SHOW);
			SetForegroundWindow(::Window.Window);
			SetFocus(::Window.Window);
			Shell_NotifyIcon(NIM_DELETE, &::Window.Notify);
		}
	}

	return 0;
}

void WINAPI CWindow::ReadPixels(GLint X, GLint Y, GLsizei Width, GLsizei Height, GLenum Format, GLenum Type, GLvoid* Pixels)
{
	//
	// Correção de screenshots não dividível por 4
	//
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	return pglReadPixels(X, Y, Width, Height, Format, Type, Pixels);
}

HFONT WINAPI CWindow::CreateFont(int Height, int Width, int Escapement, int Orientation, int Weight, DWORD Italic, DWORD Underline, DWORD StrikeOut, DWORD CharSet, DWORD OutPrecision, DWORD ClipPrecision, DWORD Quality, DWORD PitchAndFamily, LPCSTR FaceName)
{
	return pCreateFont(Height, Width, Escapement, Orientation, Weight, Italic, Underline, StrikeOut, CharSet, OutPrecision, ClipPrecision, Quality, PitchAndFamily, "Tahoma");
}

LRESULT CALLBACK CWindow::Procedure(HWND Window, UINT Code, WPARAM W, LPARAM L)
{
	switch (Code)
	{
		case WM_NCACTIVATE:
		{
			return 0;
		}
		case WM_KEYDOWN:
		{
			switch (W)
			{
				case VK_F11:
				{
					if (IsWindowVisible(::Window.Window))
					{
						ShowWindow(::Window.Window, SW_HIDE);
						Shell_NotifyIcon(NIM_ADD, &::Window.Notify);
					}

					break;
				}
			}

			break;
		}
		case WM_TRAYICON:
		{
			switch (L)
			{
				case WM_LBUTTONDBLCLK:
				case WM_RBUTTONDBLCLK:
				{
					if (!IsWindowVisible(::Window.Window))
					{
						ShowWindow(::Window.Window, SW_SHOW);
						Shell_NotifyIcon(NIM_DELETE, &::Window.Notify);
					}

					break;
				}
			}

			break;
		}
	}

	return CallWindowProc(pWndProc, Window, Code, W, L);
}

CWindow Window;