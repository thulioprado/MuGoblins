// Warehouse.h: interface for the CWarehouse class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataServerProtocol.h"

#define WAREHOUSE_SIZE 120

//**********************************************//
//********** GameServer -> DataServer **********//
//**********************************************//

struct SDHP_WAREHOUSE_ITEM_RECV
{
	PSBMSG_HEAD header; // C1:05:00
	WORD index;
	char account[11];
	UINT WarehouseNumber;
};

struct SDHP_WAREHOUSE_ITEM_SAVE_RECV
{
	PSWMSG_HEAD header; // C2:05:30
	WORD index;
	char account[11];
	BYTE WarehouseItem[WAREHOUSE_SIZE][16];
	UINT WarehouseMoney;
	UINT WarehousePassword;
	UINT WarehouseNumber;
};

//**********************************************//
//********** DataServer -> GameServer **********//
//**********************************************//

struct SDHP_WAREHOUSE_ITEM_SEND
{
	PSWMSG_HEAD header; // C2:05:00
	WORD index;
	char account[11];
	BYTE WarehouseItem[WAREHOUSE_SIZE][16];
	UINT WarehouseMoney;
	WORD WarehousePassword;
};

struct SDHP_WAREHOUSE_FREE_SEND
{
	PSBMSG_HEAD header; // C1:05:01
	WORD index;
	char account[11];
};

//**********************************************//
//**********************************************//
//**********************************************//

class CWarehouse
{
public:
	CWarehouse();
	virtual ~CWarehouse();
	void GDWarehouseItemRecv(SDHP_WAREHOUSE_ITEM_RECV* lpMsg,int index);
	void GDWarehouseItemSaveRecv(SDHP_WAREHOUSE_ITEM_SAVE_RECV* lpMsg);
	void DGWarehouseFreeSend(int ServerIndex,WORD index,char* account);
};

extern CWarehouse gWarehouse;
