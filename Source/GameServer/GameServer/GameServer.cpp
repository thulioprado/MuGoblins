#include "stdafx.h"
#include "Resource.h"
#include "GameServer.h"
#include "GameMain.h"
#include "JSProtocol.h"
#include "Message.h"
#include "MiniDump.h"
#include "Notice.h"
#include "QueueTimer.h"
#include "ServerDisplayer.h"
#include "ServerInfo.h"
#include "SocketManager.h"
#include "SocketManagerUdp.h"
#include "Util.h"

HINSTANCE hInst;
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];
HWND hWnd;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) // OK
{
	CMiniDump::Start();

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_GAMESERVER, szWindowClass, MAX_LOADSTRING);

	MyRegisterClass(hInstance);

	if (InitInstance(hInstance, nCmdShow) == 0)
	{
		return 0;
	}

	SetLargeRand();

	gServerInfo.ReadStartupInfo("GameServerInfo", ".\\Data\\GameServerInfo - Common.dat");

	char buff[256];

	wsprintf(buff, "[%s] %s (PlayerCount : %d/%d) (MonsterCount : %d/%d)", GAMESERVER_VERSION, gServerInfo.m_ServerName, gObjTotalUser, gServerInfo.m_ServerMaxUserNumber, gObjTotalMonster, MAX_OBJECT_MONSTER);

	SetWindowText(hWnd, buff);

	gServerDisplayer.Init(hWnd);

	WSADATA wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) == 0)
	{
		if (gSocketManager.Start((WORD)gServerInfo.m_ServerPort) == 0)
		{
			LogAdd(LOG_RED, "Could not start GameServer");
		}
		else
		{
			GameMainInit(hWnd);
			JoinServerConnect(WM_JOIN_SERVER_MSG_PROC);
			DataServerConnect(WM_DATA_SERVER_MSG_PROC);

			gSocketManagerUdp.Connect(gServerInfo.m_ConnectServerAddress, (WORD)gServerInfo.m_ConnectServerPort);

			SetTimer(hWnd, WM_TIMER_1000, 1000, 0);
			SetTimer(hWnd, WM_TIMER_10000, 10000, 0);

			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER, 100, &QueueTimerCallback);
			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_MOVE, 100, &QueueTimerCallback);
			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_AI, 100, &QueueTimerCallback);
			gQueueTimer.CreateTimer(QUEUE_TIMER_MONSTER_AI_MOVE, 100, &QueueTimerCallback);
			gQueueTimer.CreateTimer(QUEUE_TIMER_EVENT, 100, &QueueTimerCallback);
			gQueueTimer.CreateTimer(QUEUE_TIMER_VIEWPORT, 1000, &QueueTimerCallback);
			gQueueTimer.CreateTimer(QUEUE_TIMER_FIRST, 1000, &QueueTimerCallback);
			gQueueTimer.CreateTimer(QUEUE_TIMER_CLOSE, 1000, &QueueTimerCallback);
			gQueueTimer.CreateTimer(QUEUE_TIMER_ACCOUNT_LEVEL, 60000, &QueueTimerCallback);
		}
	}
	else
	{
		LogAdd(LOG_RED, "WSAStartup() failed with error: %d", WSAGetLastError());
	}

	gServerDisplayer.PaintAllInfo();

	SetTimer(hWnd, WM_TIMER_2000, 2000, 0);

	HACCEL hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_GAMESERVER);

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0) != 0)
	{
		if (TranslateAccelerator(msg.hwnd, hAccelTable, &msg) == 0)
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	CMiniDump::Clean();

	return msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance) // OK
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = (WNDPROC)WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, (LPCTSTR)IDI_GAMESERVER);
	wcex.hCursor = LoadCursor(0, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = (LPCSTR)IDC_GAMESERVER;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) // OK
{
	hInst = hInstance;

	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_THICKFRAME, CW_USEDEFAULT, 0, 600, 600, 0, 0, hInstance, 0);

	if (hWnd == 0)
	{
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return 1;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) // OK
{
	switch (message)
	{
		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case IDM_ABOUT:
					DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
					break;
				case IDM_EXIT:
					if (MessageBox(0, "Are you sure to terminate GameServer?", "Ask terminate server", MB_YESNO | MB_ICONQUESTION) == IDYES)
					{
						DestroyWindow(hWnd);
					}
					break;
				case IDM_FILE_ALLUSERLOGOUT:
					gObjAllLogOut();
					break;
				case IDM_FILE_ALLUSERDISCONNECT:
					gObjAllDisconnect();
					break;
				case IDM_FILE_1MINUTESERVERCLOSE:
					if (gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 60;
						gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(487));
					}
					break;
				case IDM_FILE_3MINUTESERVERCLOSE:
					if (gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 180;
						gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(488));
					}
					break;
				case IDM_FILE_5MINUTESERVERCLOSE:
					if (gCloseMsg == 0)
					{
						gCloseMsg = 1;
						gCloseMsgTime = 300;
						gNotice.GCNoticeSendToAll(0, 0, 0, 0, 0, 0, gMessage.GetMessage(489));
					}
					break;
				case IDM_RELOAD_RELOADCHAOSMIX:
					gServerInfo.ReadChaosMixInfo();
					break;
				case IDM_RELOAD_RELOADCHARACTER:
					gServerInfo.ReadCharacterInfo();
					break;
				case IDM_RELOAD_RELOADCOMMAND:
					gServerInfo.ReadCommandInfo();
					break;
				case IDM_RELOAD_RELOADCOMMON:
					gServerInfo.ReadCommonInfo();
					break;
				case IDM_RELOAD_RELOADCUSTOM:
					gServerInfo.ReadCustomInfo();
					break;
				case IDM_RELOAD_RELOADEVENT:
					gServerInfo.ReadEventInfo();
					break;
				case IDM_RELOAD_RELOADEVENTITEMBAG:
					gServerInfo.ReadEventItemBagInfo();
					break;
				case IDM_RELOAD_RELOADHACK:
					gServerInfo.ReadHackInfo();
					break;
				case IDM_RELOAD_RELOADITEM:
					gServerInfo.ReadItemInfo();
					break;
				case IDM_RELOAD_RELOADMONSTER:
					gServerInfo.ReloadMonsterInfo();
					break;
				case IDM_RELOAD_RELOADMOVE:
					gServerInfo.ReadMoveInfo();
					break;
				case IDM_RELOAD_RELOADQUEST:
					gServerInfo.ReadQuestInfo();
					break;
				case IDM_RELOAD_RELOADSHOP:
					gServerInfo.ReadShopInfo();
					break;
				case IDM_RELOAD_RELOADSKILL:
					gServerInfo.ReadSkillInfo();
					break;
				case IDM_RELOAD_RELOADUTIL:
					gServerInfo.ReadUtilInfo();
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_TIMER:
			switch (wParam)
			{
				case WM_TIMER_1000:
					GJServerUserInfoSend();
					ConnectServerInfoSend();
					break;
				case WM_TIMER_2000:
					gObjCountProc();
					gServerDisplayer.Run();
					break;
				case WM_TIMER_10000:
					JoinServerReconnect(hWnd, WM_JOIN_SERVER_MSG_PROC);
					DataServerReconnect(hWnd, WM_DATA_SERVER_MSG_PROC);
					break;
			}
			break;
		case WM_JOIN_SERVER_MSG_PROC:
			JoinServerMsgProc(wParam, lParam);
			break;
		case WM_DATA_SERVER_MSG_PROC:
			DataServerMsgProc(wParam, lParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) // OK
{
	switch (message)
	{
		case WM_INITDIALOG:
			return 1;
		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			{
				EndDialog(hDlg, LOWORD(wParam));
				return 1;
			}
			break;
	}

	return 0;
}
