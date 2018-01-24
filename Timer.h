#ifndef _TIMER_H
#define _TIMER_H

//IncludeFiles

#include "UObject.h"

class Timer : public UObject
{
public:
	//Constructor
	Timer(const std::tstring& name);
	//Destructor
	virtual ~Timer();

	//Copy Constructor & Assignement operator
	Timer(const Timer& tRef);
	Timer& operator = (const Timer& tRef);

	//void* operator new(size_t size);
	//void operator delete(void* pdelete);

	//Methods
	bool Initialize();
	bool PostInitialize();
	bool Update(Context& context);
	bool ShutDown();

	float GetTotalTime();
	float GetDeltaTime();
	
	void Reset();

private:
	//Methods
	void Start();
	void Stop();

	//Members
	double m_SecondsPerCount;
	double m_DeltaTime;

	_int64 m_BaseTime;
	_int64 m_PauseTime;
	_int64 m_StopTime;
	_int64 m_PrevTime;
	_int64 m_CurrTime;

	bool m_bStopped;

};

#endif