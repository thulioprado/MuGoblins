#pragma once

//
// Brilho/Cores
//
struct GlowColor
{
	float Red;
	float Green;
	float Blue;
};

//
// Prisma
//
struct PrismEffect
{
	float Red;
	float Green;
	float Blue;
};

//
// Jogador
//
#define PLAYER_OBJECT_BASE					*(DWORD*)(0x7666A78)

//
// Viewport
//
#define VIEWPORT_BASE						*(DWORD*)(0x7424644)
#define VIEWPORT_MAX						400
#define VIEWPORT_USER						1
#define VIEWPORT_MONSTER					2

//
// Guild
//
#define GUILD_RELATIONSHIP_UNION			1
#define GUILD_RELATIONSHIP_RIVAL			2
#define GUILD_RELATIONSHIP_UNIONMASTER		4
#define GUILD_RELATIONSHIP_RIVALMASTER		8

//
// Estado
//
#define STATE_SELECT_SERVER					2 
#define STATE_SELECT_CHARACTER				4
#define STATE_PLAYING						5

//
// Monstros
//
#define MONSTER_NAME_BASE					0x7666A80

//
// Variáveis
//
#define pHDC								*(HDC*)(0x57504DC)
#define pDefaultFont						*(HFONT*)(0x575053C)
#define pMediumFont 						*(HFONT*)(0x5750540)
#define pBigFont							*(HFONT*)(0x5750544)
#define pSocket								*(SOCKET*)(*(DWORD*)(0x6B7264) + 12)
#define pSerialPacket		 				*(BYTE*)(0x57548F3)
#define pScreenDivisorX 					*(float*)(0x574FE90)
#define pScreenDivisorY 					*(float*)(0x574FE94)
#define pScreenWidth						*(DWORD*)(0x6C4158)
#define pScreenHeight 						*(DWORD*)(0x6C415C)
#define pScreenWidthBase 					640
#define pScreenHeightBase					480
#define pResolution							*(DWORD*)(0x5750320)
#define pFontSize							*(DWORD*)(0x773D130)
#define pClipPrecisionX						*(float*)(0x6A967C)
#define pClipPrecisionY		 				*(float*)(0x5E9D4E)
#define pState								*(DWORD*)(0x6C41C0)
#define pMessageBackgroundColor				*(DWORD*)(0x6B7DF8)
#define pMessageForegroundColor				*(DWORD*)(0x6B7DF0)
#define pCurrentMessage					 	*(DWORD*)(0x788C850)
#define pMapNumber							(BYTE)(*(DWORD*)(0x6B8D48))
#define pMyViewport							*(DWORD*)(0x742464C)
#define pCameraZoom							*(float*)(0x626681)
#define pCameraRotY							*(float*)(0x6A9798)
#define pCameraRotZ							*(float*)(0x7D171A0)
#define pCameraPosZ							*(float*)(0x6A7770)
#define pCameraClipX 						*(float*)(0x6A967C)
#define pCameraClipX2 						*(float*)(0x6A9688)
#define pCameraClipY 						*(float*)(0x5E9D4E)
#define pCameraClipY2						*(float*)(0x5E9CFE)
#define pCameraClipB 						*(float*)(0x6A9680)
#define pCameraClipGL 						*(float*)(0x62696E)
#define pCameraClipUnk1						*(float*)(0x6A9684)
#define pCameraClipUnk2						*(float*)(0x6A8FBC)
#define pConnectServerPort					*(WORD*)(0x6C41BC)
#define pConnectServerIpAddress				(char*)(0x6B6694)
#define pClientVersion						(char*)(0x6B7248)
#define pClientSerial						(char*)(0x6B7250)
#define pClientScreenshot					(char*)(0x6C4958)
#define pReconnectAccount					(char*)(0x777D7C0)
#define pReconnectPassword					(char*)(0x777D8C0)
#define pPlayerState						*(DWORD*)(0x57548F8)
#define pReconnectClear1					*(int*)(0x77D86D8)
#define pReconnectClear2					*(int*)(0x6B7D60)
#define pReconnectClear3					*(int*)(0x710C30)
#define pReconnectClear4					*(int*)(0x788C82C)
#define pReconnectClear5					*(int*)(0x788C830)
#define pReconnectClear6					*(int*)(0x6B7DBC)
#define pReconnectClear7					*(int*)(0x6B7DC0)
#define pReconnectClear8					*(int*)(0x6B7DC4)
#define pReconnectClear9					*(int*)(0x6B7DC8)
#define pReconnectClear10					*(int*)(0x6B7DCC)
#define pReconnectClear11					*(BYTE*)(0x6B7DE1)
#define pCharacterOpen						*(DWORD*)(0x77D83A8)
#define pCommandOpen						*(DWORD*)(0x77D83AC)
#define pInventoryOpen						*(DWORD*)(0x77D83F4)
#define pPartyOpen							*(DWORD*)(0x77D83DC)
#define pGuildOpen							*(DWORD*)(0x77D83B8)
#define pFriendOpen							*(DWORD*)(0x77D83B4)
#define pQuestOpen							*(DWORD*)(0x77D83EC)
#define pMoveOpen							*(DWORD*)(0x77D83D0)
#define pImageDivisor						0.03125f
#define pMaxTexture							*(DWORD*)(0x7D16FE8)
#define pLockMain							*(BYTE*)(0x575491C)
#define pModelPositionX						*(float*)(0x788BAFC)
#define pModelPositionY						*(float*)(0x788BB00)
#define pModelPositionZ						*(float*)(0x788BB04)
#define pCursorInterface					*(DWORD*)(0x788C848)
#define pCursorSlot							*(DWORD*)(0x788C84C)

//
// Funções
//
#define pWndProc							(WNDPROC)(0x4A9BD0)
#define pProtocolCore						((int(*)(DWORD, LPBYTE, int, int))(0x4DB720))
#define pDrawDamage							((void(*)(int, int, int, int, char))(0x53DA80))
#define pUnkFormatPoint						((void(*)(int, float, float, float, float, float, float, float, float, char, int))(0x60DCF0))
#define pDrawMonsterHP						((void(*)())(0x598350))
#define pGetPositionFromAngle				((void(*)(LPVOID, int*, int*))(0x60C740))
#define pSetBlend							((void(__cdecl*)(BYTE)) 0x60CB90)
#define pEnableAlpha						((void(*)(char))(0x60CB90))
#define pDisableAlpha						((void(*)())(0x60CC20))
#define pDisableAlphaBlend					((void(*)())(0x60CB10))
#define pDrawForm							((void(*)(float, float, float, float))(0x60DC30))
#define pDrawText							((int(*)(int, int, LPCSTR, int, char, int))(0x5486E0))
#define pDrawUserInfo						((void(*)(int, int, float, float, int, int, float, int))(0x5484B0))
#define pDrawImage							((void(*)(int, float, float, float, float, float, float, float, float, char, float))(0x60DCF0))
#define pSerialPacketEncrypt()				((BYTE(__thiscall*)(void*, const char*))(0x409580))((void*)(0x57500B0), (const char*)(0x57548F3))
#define pSerialPacketDecrypt()				((BYTE(__thiscall*)(void*, const char*))(0x409340))((void*)(0x57500B0), (const char*)(0x57548F3))
#define pSimpleModulusEncrypt(a, b, c)		((int(__thiscall*)(void*, void*, void*, int))(0x674DC0))((void*)(0x5754800), a, b, c)
#define pSimpleModulusDecrypt(a, b, c)		((int(__thiscall*)(void*, void*, void*, int))(0x674E30))((void*)(0x5754848), a, b, c)
#define pSetGlow							((void(*)(int, float, DWORD, GlowColor*, DWORD))(0x5F6220))
#define pShowMessage						((char(*)(const char*, const char*, int))(0x549630))
#define pRenderizeMonster					((ViewportInfo*(*)(int, DWORD, int, int))(0x434E60))
#define pRenderizeModel						((ViewportInfo*(*)(int, int, DWORD, int, int))(0x509410))
#define pShowModel							((int(*)(int, int, char, int, float*, int, char))(0x5CF110))
#define pLoadModel							((void(*)(int, const char*, const char*, int))(0x5FACA0))
#define pLoadTexture						((void(*)(int, const char*, GLint, GLint, GLsizei))(0x5FA860))
#define pLoadModels							((void(*)())(0x5FD0E0))
#define pLoadTextures						((void(*)())(0x5FE910))
#define pLoadInterfaces						((void(*)())(0x6096B0))
#define pLoadJPG							((char(*)(const char*, int, int, int, int, char))(0x62C1E0))
#define pLoadTGA							((char(*)(const char*, int, int, int, int, int))(0x62C670))
#define pLoadSounds							((void(*)())(0x607CE0))
#define pConnect							((int(*)(LPCSTR, WORD))(0x4BBA20))
#define pSetEvent							((int(*)(DWORD))(0x60FB20))
#define pInterfaceDraw1						((void(*)(int))(0x598C30))
#define pPShopThis							((LPVOID(*)())(0x44C850))
#define pPShopSet(viewport)					((void(__thiscall*)(LPVOID, ViewportInfo*))(0x44BF70))(pPShopThis(), viewport)
#define pRefreshGame						((int(*)(ViewportInfo*))(0x4EA700))
#define pFixText()							((void(*)(int, int, LPCSTR))(0x5487E0))(0, 0, "")
#define pCreateSign							((LPBYTE(__thiscall*)(LPVOID, short, int, char, float, float, float, int, int, float))(0x4127F0))
#define pCreateSign2						((LPBYTE(__thiscall*)(LPVOID, short, int, int, float, float, float))(0x412C40))
#define pDestroySign						((void(*)(ViewportInfo*))(0x4127C0))
#define pGetEffectPosition					((int(__thiscall*)(DWORD, DWORD, DWORD, DWORD, char))(0x4E4610))
#define pShowEffect1						((int(*)(short, DWORD, float, float*, DWORD, DWORD, DWORD))(0x53DE20))
#define pShowEffect2						((int(*)(int, DWORD, DWORD, float*, DWORD, float, DWORD))(0x5353B0))
#define pMakePreviewCharacter				((void(*)(int, BYTE*, int, int))(0x50A590))
#define pCheckInterface(code)				((bool(__stdcall*)(void*))(0x648BB0))((void*)(code))
#define pRenderModel						((void(*)(float, float, float, int, float, int))(0x5E5D0B))
#define pRenderizeEquippedItems				((void(*)())(0x5D1760))

//
// Macros
//
#define GET_ALLIANCE_NAME(i)				((char*)(2 * (11 * i) + 0x785B794))
#define GET_MAX_WORD_VALUE(x)				((x > 65000) ? 65000 : (WORD)(x))
#define SET_BYTE(x, y)						(BYTE((y & 0x0F) | ((int(x) << 4) & 0xF0)))
#define GET_NIBBLE_X(v)						(BYTE((int(v) & 0xF0) >> 4))
#define GET_NIBBLE_Y(v)						(BYTE(v & 0x0F))
#define RGBA(R, G, B, A)					(A << 24) | (B << 16) | (G << 8) | R

//
// Itens
//
#define	MAX_ITEM_INFO						16
#define MAX_ITEM_INFO_MAIN					7
#define GET_ITEM(group, index)				(group * 512) + index
#define GET_ITEM_MODEL(group, index)		GET_ITEM(group, index) + 515

// 
// Outros
//
#define CHAR_SET_SIZE						24
#define CHAR_SET_SIZE_MAIN					18
#define INVENTORY_SIZE						108
#define TRADE_SIZE							32
#define WAREHOUSE_SIZE						120
#define CHAOS_BOX_SIZE						32
#define PERSONAL_SHOP_SIZE					32

namespace TargetFlag
{
	enum Value
	{
		Inventory = 0,
		Trade = 1,
		Warehouse = 2,
		ChaosMachine = 3,
		PersonalShop = 4,
		Trainer = 5,
	};
}

namespace CursorInterface
{
	enum Value
	{
		Inventory = 0,
		Warehouse = 1,
		TradeSource = 2,
		TradeTarget = 3,
		ChaosMachine = 4,
		PersonalShop = 5,
		Shop = 6
	};
}