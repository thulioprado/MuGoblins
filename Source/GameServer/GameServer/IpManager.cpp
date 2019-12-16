// IpManager.cpp: implementation of the CIpManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IpManager.h"
#include "ServerInfo.h"

CIpManager gIpManager;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIpManager::CIpManager() // OK
{

}

CIpManager::~CIpManager() // OK
{

}

bool CIpManager::CheckIpAddress(char* IpAddress) // OK
{
	auto it = this->m_IpAddressInfo.find(std::string(IpAddress));

	if(it == this->m_IpAddressInfo.end())
	{
		return ((gServerInfo.m_MaxIpConnection==0)?0:1);
	}
	else
	{
		return ((it->second.IpAddressCount>=gServerInfo.m_MaxIpConnection)?0:1);
	}
}

void CIpManager::InsertIpAddress(char* IpAddress) // OK
{
	IP_ADDRESS_INFO info;

	strcpy_s(info.IpAddress,IpAddress);

	info.IpAddressCount = 1;

	auto it = this->m_IpAddressInfo.find(std::string(IpAddress));

	if(it == this->m_IpAddressInfo.end())
	{
		this->m_IpAddressInfo[IpAddress] = info;
	}
	else
	{
		++it->second.IpAddressCount;
	}
}

void CIpManager::RemoveIpAddress(char* IpAddress) // OK
{
	auto it = this->m_IpAddressInfo.find(std::string(IpAddress));

	if(it != this->m_IpAddressInfo.end())
	{
		if((--it->second.IpAddressCount) == 0)
		{
			this->m_IpAddressInfo.erase(it);
		}
	}
}
