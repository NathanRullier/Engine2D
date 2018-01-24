#ifndef _ENGINE_H
#define _ENGINE_H

// C Runtime Header Files
#ifndef _WINDOWS_
#include <windows.h>
#endif

#ifndef MAP
#include <map>
#endif

#ifndef _CONTEXT_H
#include "context.h"
#endif

//Forward Declaration
enum SystemType;
class System;
class Game;

//Enumeration
enum EngineState
{
	Invalid,
	Constructing,
	Initializing,
	Running,
	ShuttingDown,
	Destroying

};
class Engine
{
public:
	Engine();
	~Engine();

	int RunLoop();

	//void* operator new(size_t size);
	//void operator delete(void* pdelete); 
	 
	static EngineState getEngineState() {
		return m_EngineState;
	}
private :

	int Initialize();
	int Draw(Context& context); 
	int Update(Context& context);
	int ShutDown();
	
	//Check if we need to resize the sreen
	void CheckResize();
	//Add a core System to the engine
	int AddSystem(System* psys);
	//Retrieve a core System from the engine
	template<typename T>
	T* getSystem(SystemType systype) {
		T* psys = static_cast<T*>(m_mapSystems[systype]);
			if (!psys) {
				//Logger::log("System is not valid");
				return nullptr;
			}
			else {
				return psys;
			}
	}
	
	//Create the game instance 
	Game* CreateGame();

	std::map<SystemType, System*> m_mapSystems;
	static EngineState m_EngineState;
};
#endif