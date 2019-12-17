// Map.h: interface for the CMap class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "MapItem.h"
#include "MapPath.h"

#define MAX_MAP 57
#define MAX_MAP_ITEM 300
#define MAX_MAP_WIDTH 256
#define MAX_MAP_HEIGHT 256

#define MAP_RANGE(x) (((x)<0)?0:((x)>=MAX_MAP)?0:1)
#define MAP_ITEM_RANGE(x) (((x)<0)?0:((x)>=MAX_MAP_ITEM)?0:1)

#define KALIMA_MAP_RANGE(x) ((x>=MAP_KALIMA1 && x<=MAP_KALIMA6)?1:((x==MAP_KALIMA7)?1:0))
#define DS_MAP_RANGE(x) ((x==MAP_DEVIL_SQUARE1)?1:(x==MAP_DEVIL_SQUARE2)?1:0)
#define BC_MAP_RANGE(x) ((x>=MAP_BLOOD_CASTLE1 && x<=MAP_BLOOD_CASTLE7)?1:0)
#define CC_MAP_RANGE(x) ((x>=MAP_CHAOS_CASTLE1 && x<=MAP_CHAOS_CASTLE6)?1:0)

enum eMapNumber
{
	MAP_LORENCIA = 0,
	MAP_DUNGEON = 1,
	MAP_DEVIAS = 2,
	MAP_NORIA = 3,
	MAP_LOST_TOWER = 4,
	MAP_RESERVED1 = 5,
	MAP_ARENA = 6,
	MAP_ATLANS = 7,
	MAP_TARKAN = 8,
	MAP_DEVIL_SQUARE1 = 9,
	MAP_ICARUS = 10,
	MAP_BLOOD_CASTLE1 = 11,
	MAP_BLOOD_CASTLE2 = 12,
	MAP_BLOOD_CASTLE3 = 13,
	MAP_BLOOD_CASTLE4 = 14,
	MAP_BLOOD_CASTLE5 = 15,
	MAP_BLOOD_CASTLE6 = 16,
	MAP_BLOOD_CASTLE7 = 17,
	MAP_CHAOS_CASTLE1 = 18,
	MAP_CHAOS_CASTLE2 = 19,
	MAP_CHAOS_CASTLE3 = 20,
	MAP_CHAOS_CASTLE4 = 21,
	MAP_CHAOS_CASTLE5 = 22,
	MAP_CHAOS_CASTLE6 = 23,
	MAP_KALIMA1 = 24,
	MAP_KALIMA2 = 25,
	MAP_KALIMA3 = 26,
	MAP_KALIMA4 = 27,
	MAP_KALIMA5 = 28,
	MAP_KALIMA6 = 29,
	MAP_CASTLE_SIEGE = 30,
	MAP_LAND_OF_TRIALS = 31,
	MAP_DEVIL_SQUARE2 = 32,
	MAP_AIDA = 33,
	MAP_CRYWOLF = 34,
	MAP_RESERVED2 = 35,
	MAP_KALIMA7 = 36
};

class CMap
{
public:
	CMap();
	virtual ~CMap();
	void Load(char* path,int map);
	BYTE GetAttr(int x,int y);
	bool CheckAttr(int x,int y,BYTE attr);
	void SetAttr(int x,int y,BYTE attr);
	void DelAttr(int x,int y,BYTE attr);
	bool CheckStandAttr(int x,int y);
	void SetStandAttr(int x,int y);
	void DelStandAttr(int x,int y);
	BYTE GetWeather();
	void SetWeather(BYTE weather,BYTE variation);
	void WeatherVariationProcess();
	void WeatherAllSend(BYTE weather);
	void GetMapPos(int map,short* ox,short* oy);
	void GetMapRandomPos(short* ox,short* oy,int size);
	bool MoneyItemDrop(int money,int x,int y);
	bool MonsterItemDrop(int index,int level,float dur,int x,int y,BYTE Option1,BYTE Option2,BYTE Option3,BYTE NewOption,BYTE SetOption,int aIndex,DWORD serial,DWORD duration);
	bool ItemDrop(int index,int level,float dur,int x,int y,BYTE Option1,BYTE Option2,BYTE Option3,BYTE NewOption,BYTE SetOption,DWORD serial,int aIndex,int PetLevel,int PetExp,DWORD duration);
	bool CheckItemGive(int aIndex,int index);
	void ItemGive(int aIndex,int index);
	void StateSetDestroy();
	BOOL CheckWall(int sx,int sy,int tx,int ty);
	BYTE CheckWall2(int sx,int sy,int tx,int ty);
	bool PathFinding2(int sx,int sy,int tx,int ty,PATH_INFO* path);
	bool PathFinding3(int sx,int sy,int tx,int ty,PATH_INFO* path);
	bool PathFinding4(int sx,int sy,int tx,int ty,PATH_INFO* path);
public:
	BYTE m_Weather;
	BYTE m_WeatherVariation;
	DWORD m_WeatherTimer;
	DWORD m_NextWeatherTimer;
	BYTE* m_MapAttr;
	int m_width;
	int m_height;
	CMapPath* m_path;
	CMapItem m_Item[MAX_MAP_ITEM];
	RECT m_MapRespawn[MAX_MAP];
	int m_MapNumber;
	int m_ItemCount;
};

extern CMap gMap[MAX_MAP];
