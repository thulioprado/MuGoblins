// Warehouse.cpp: implementation of the CWarehouse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Warehouse.h"
#include "QueryManager.h"
#include "SocketManager.h"

CWarehouse gWarehouse;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWarehouse::CWarehouse() // OK
{

}

CWarehouse::~CWarehouse() // OK
{

}

void CWarehouse::GDWarehouseItemRecv(SDHP_WAREHOUSE_ITEM_RECV* lpMsg, int index) // OK
{
	SDHP_WAREHOUSE_ITEM_SEND pMsg;

	pMsg.header.set(0x05, 0x00, sizeof(pMsg));

	pMsg.index = lpMsg->index;

	memcpy(pMsg.account, lpMsg->account, sizeof(pMsg.account));
	
	if (gQueryManager.ExecQuery("SELECT Items, Money, pw FROM WarehouseItems WHERE AccountID = '%s' AND Number = %d", lpMsg->account, lpMsg->WarehouseNumber) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("INSERT INTO WarehouseItems (AccountID, Number, DbVersion) VALUES ('%s', %d, 3)", lpMsg->account, lpMsg->WarehouseNumber);
		gQueryManager.Close();

		gQueryManager.ExecQuery("UPDATE WarehouseItems SET Items = CONVERT(varbinary(%d), REPLICATE(char(0xFF), %d)) WHERE AccountID = '%s' AND Number = %d", sizeof(pMsg.WarehouseItem), sizeof(pMsg.WarehouseItem), lpMsg->account, lpMsg->WarehouseNumber);
		gQueryManager.Close();

		this->DGWarehouseFreeSend(index, lpMsg->index, lpMsg->account);
		return;
	}

	gQueryManager.GetAsBinary("Items", pMsg.WarehouseItem[0], sizeof(pMsg.WarehouseItem));

	pMsg.WarehouseMoney = gQueryManager.GetAsInteger("Money");
	pMsg.WarehousePassword = gQueryManager.GetAsInteger("pw");
	
	gQueryManager.Close();

	gSocketManager.DataSend(index, (BYTE*)&pMsg, sizeof(pMsg));
}

void CWarehouse::GDWarehouseItemSaveRecv(SDHP_WAREHOUSE_ITEM_SAVE_RECV* lpMsg) // OK
{
	if (gQueryManager.ExecQuery("SELECT AccountID FROM WarehouseItems WHERE AccountID = '%s' AND Number = %d", lpMsg->account, lpMsg->WarehouseNumber) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.BindParameterAsBinary(1, lpMsg->WarehouseItem[0], sizeof(lpMsg->WarehouseItem));
		gQueryManager.ExecQuery("INSERT INTO WarehouseItems (Items, AccountID, Number, DbVersion, Money, pw) VALUES (?, '%s', %d, 3, %d, %d)", lpMsg->account, lpMsg->WarehouseNumber, lpMsg->WarehouseMoney, lpMsg->WarehousePassword);
		gQueryManager.Close();
	}
	else
	{
		gQueryManager.Close();

		gQueryManager.BindParameterAsBinary(1, lpMsg->WarehouseItem[0], sizeof(lpMsg->WarehouseItem));
		gQueryManager.ExecQuery("UPDATE WarehouseItems SET Items = ?, Money = %d, pw = %d WHERE AccountID = '%s' AND Number = %d", lpMsg->WarehouseMoney, lpMsg->WarehousePassword, lpMsg->account, lpMsg->WarehouseNumber);
		gQueryManager.Close();
	}
}

void CWarehouse::DGWarehouseFreeSend(int ServerIndex, WORD index, char* account) // OK
{
	SDHP_WAREHOUSE_FREE_SEND pMsg;

	pMsg.header.set(0x05, 0x01, sizeof(pMsg));

	pMsg.index = index;

	memcpy(pMsg.account, account, sizeof(pMsg.account));

	gSocketManager.DataSend(ServerIndex, (BYTE*)&pMsg, pMsg.header.size);
}

void CWarehouse::GDWarehouseInfoRecv(SDHP_WAREHOUSE_INFO_RECV* lpMsg, int index)
{
	this->DGWarehouseInfoSend(index, lpMsg->index, lpMsg->account);
}

void CWarehouse::DGWarehouseInfoSend(int ServerIndex, WORD index, char* account)
{
	SDHP_WAREHOUSE_INFO_SEND pMsg;

	pMsg.header.set(0x05, 0x02, sizeof(pMsg));

	memcpy(pMsg.account, account, sizeof(pMsg.account));

	pMsg.index = index;
	pMsg.WarehouseNumber = 0;
	pMsg.WarehouseTotal = 1;

	if (gQueryManager.ExecQuery("SELECT Number, Total FROM Warehouse WHERE AccountID = '%s'", account) == 0 || gQueryManager.Fetch() == SQL_NO_DATA)
	{
		gQueryManager.Close();

		gQueryManager.ExecQuery("INSERT INTO Warehouse (AccountID, EndUseDate) VALUES ('%s', getdate())", account);
		gQueryManager.Close();
	}
	else
	{
		pMsg.WarehouseNumber = gQueryManager.GetAsInteger("Number");
		pMsg.WarehouseTotal = gQueryManager.GetAsInteger("Total");

		gQueryManager.Close();
	}

	gSocketManager.DataSend(ServerIndex, (BYTE*)&pMsg, pMsg.header.size);
}
