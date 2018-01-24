#include "Timer.h"

#ifndef _WINDOWs_
#include <Windows.h>
#endif

//#include "MemoryManager.h"
//#include "Logger.h"

#ifndef _CTIME_
#include <ctime>
#endif

Timer::Timer(const std::tstring& name)
	:UObject(name)

	, m_SecondsPerCount(0.0)
	, m_DeltaTime(-1.0)

	, m_BaseTime(0)
	, m_PauseTime(0)
	, m_StopTime(0)
	, m_PrevTime(0)
	, m_CurrTime(0)

	, m_bStopped(false)
{
	_int64 countpersec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countpersec);
	m_SecondsPerCount = 1.0 / (double)countpersec;
}


Timer::~Timer()
{
}

//void* Timer::operator new(size_t size) 
//{
//	return MEMORYMANAGER->Alloc(size);
//}
//void Timer::operator delete(void* pdelete) 
//{
//	MEMORYMANAGER->free(pdelete);
//}
bool Timer::Initialize()
{
	UObject::Initialize();

	Start();

	return true;
}
bool Timer::PostInitialize()
{
	return UObject::PostInitialize();
}
bool Timer::Update(Context& context)
{
	UObject::Update(context);

	if (m_bStopped)
	{
		m_DeltaTime = 0.0f;
		return true;
	}
	//get the time this frame
	_int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_CurrTime = currTime;

	//time difference beetween this frame and the previous one;
	m_DeltaTime = (m_CurrTime - m_PrevTime)*m_SecondsPerCount;

	//Prepare the next frame
	m_PrevTime = m_CurrTime;
	
	//Force nonnegative
	//The DXSKS's CDXUTTimer metions that if the processor goes into power saving mode or we get shuffled to another processor then deltatime can be negative
	if (m_DeltaTime < 0.0)
		m_DeltaTime = 0.0;

	return true;
}
bool Timer::ShutDown()
{
	UObject::ShutDown();

	Stop();

	return true;
}

float Timer::GetTotalTime()
{
	if (m_bStopped)
		return(float)((m_StopTime - m_BaseTime) * m_SecondsPerCount); 

	else return(float)(((m_CurrTime - m_PauseTime) - m_BaseTime)*m_SecondsPerCount);
}
float Timer::GetDeltaTime()
{
	return (float)m_DeltaTime;
}

void Timer::Reset()
{
	_int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;

	m_bStopped = false;
}

void Timer::Start()
{
	_int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
	m_PrevTime = startTime;

	//If we are resuming a timer from a stopped state
	if (m_bStopped)
	{
		//then accumulate the past time
		m_PauseTime += (startTime - m_StopTime);

		//Since we are starting the timer back up the current previous time is not valid as it occured while paused
		//So reset the current time
		m_PrevTime = startTime;

		//no longer stopped
		m_StopTime = 0;
		m_bStopped = false;

	}
}
void Timer::Stop()
{
	//if we are already stopped then don't do anything

	if (m_bStopped)
		return;

	_int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	//Save the time we stopped at and set the boolean flag indication the timer stopped
	m_StopTime = currTime;
	m_bStopped = false;

}