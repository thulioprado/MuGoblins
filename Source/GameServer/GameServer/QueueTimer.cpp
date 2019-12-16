// QueueTimer.cpp: implementation of the CQueueTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QueueTimer.h"

CQueueTimer gQueueTimer;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQueueTimer::CQueueTimer() // OK
{
	this->m_QueueTimer = CreateTimerQueue();

	this->m_QueueTimerInfo.clear();
}

CQueueTimer::~CQueueTimer() // OK
{
	DeleteTimerQueue(this->m_QueueTimer);

	this->m_QueueTimerInfo.clear();
}

void CQueueTimer::CreateTimer(int TimerIndex,int TimerDelay,WAITORTIMERCALLBACK CallbackFunction) // OK
{
	QUEUE_TIMER_INFO QueueTimerInfo;

	QueueTimerInfo.TimerIndex = TimerIndex;

	CreateTimerQueueTimer(&QueueTimerInfo.QueueTimerTimer,this->m_QueueTimer,CallbackFunction,(PVOID)TimerIndex,1000,TimerDelay,WT_EXECUTEINTIMERTHREAD);

	this->m_QueueTimerInfo[QueueTimerInfo.TimerIndex] = QueueTimerInfo;
}

void CQueueTimer::DeleteTimer(int TimerIndex) // OK
{
	auto it = this->m_QueueTimerInfo.find(TimerIndex);

	if(it != this->m_QueueTimerInfo.end())
	{
		DeleteTimerQueueTimer(this->m_QueueTimer,it->second.QueueTimerTimer,0);

		this->m_QueueTimerInfo.erase(it);
	}
}
