#include "EngineTimer.h"

#include "Timer.h"

#ifndef _DELETEMACROS_H
#include "deletemacros.h"
#endif

//#include "MemoryManager.h"
//#include "Logger.h"

EngineTimer::EngineTimer(const EngineTimerData& data)
	:System(data)
{

}
EngineTimer::~EngineTimer()
{
}

//void* EngineTimer::operator new(size_t size) 
//{
//	return MEMORYMANAGER->Alloc(size);
//}
//void EngineTimer::operator delete(void* pdelete) 
//{
//	MEMORYMANAGER->free(pdelete);
//}

bool EngineTimer::Initialize()
{
	System::Initialize();

	m_pWorldTimer = new Timer(_T("WorldTimer"));
#if defined(DEBUG) | defined(_DEBUG)
	if (!m_pWorldTimer->Initialize())
		return false;
	//m_pWorldTimer->SetInitialized();
	//if (!m_pWorldTimer->PostInitialized())
	//	return false; 
	//m_pWorldTimer->SetPostInitialized();
#else
	m_pWorldTimer->Initialize();
	//m_pWorldTimer->SetInitialized();
	m_pWorldTimer->PostInitialized();
	// m_pWorldTimer->SetPostInitialized();
#endif
	//Logger:Log(_T("Engine Timer Initialized"),LOGTYPE_INFO,false);
}
bool EngineTimer::Update(Context& context) 
{
	System::Update(context);

	if (m_pWorldTimer)
	{
		m_pWorldTimer->Update(context);
		context.dTime = m_pWorldTimer->GetDeltaTime();
	}

	for (Timer* timer : m_vecTimers)
		timer->Update(context);

	return true;
}
bool EngineTimer::ShutDown()
{
	System::ShutDown();

	SafeDelete(m_pWorldTimer);

	for (Timer* timer : m_vecTimers)
		SafeDelete(timer);

	return true;
}