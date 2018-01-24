#include "Engine.h"
EngineState Engine::m_EngineState = EngineState::Invalid;

//Aditionnal include files
#include "Renderer.h"

#include "System.h"
#include "Game.h"
#include "Window.h"
#include "Graphic.h"
#include"EngineTimer.h"

#include "GraphicsDeviceMaganager.h"

#ifndef _DELETEMACRO_H
#include "deletemacros.h"
#endif

#ifndef _STRING_H
#include "string.h"
#endif

Engine::Engine()
{
	m_EngineState = EngineState::Constructing;
}
Engine::~Engine()
{
	m_EngineState = EngineState::Destroying;
}
//Public Methods
#pragma region Public Methods
int Engine::RunLoop()
{
	Context context;
	context.pRenderer = new Renderer();

	if (!this->Initialize())
		return 0;

	srand(GetTickCount());
		
	MSG msg = {};

	m_EngineState = EngineState::Running;

	while (msg.message != WM_QUIT && m_EngineState == EngineState::Running) {
		//CheckResize();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		this->Update(context);
		this->Draw(context);
			
	}
	//Logger::Log("Ending the Program");
	//Logger::WriteLogFile();

	if (!this->ShutDown())
		return 0;

	return msg.wParam;
}
#pragma endregion

//Private Methods
#pragma region Private Methods
int Engine::Initialize()
{
	m_EngineState = EngineState::Initializing;

	Game* game = CreateGame();

	if (!game)
		return false;

	//Add some systems
	if (!AddSystem(new Window(WindowData(640, 480))))
	return false;
	if (!AddSystem(new Graphic(GraphicData(getSystem<Window>(SystemType::Sys_Window)))))
		return false;
	if (!AddSystem(new EngineTimer(EngineTimerData())))
		return false;

	//Initialize the system
	if (!m_mapSystems[SystemType::Sys_Window]->Initialize())
		return false;
	if (!m_mapSystems[SystemType::Sys_Graphics]->Initialize())
		return false;
	if (!m_mapSystems[SystemType::Sys_EngineTimer]->Initialize())
		return false;

	GRAPHICSDEVICEMANAGER->SetGraphic(getSystem<Graphic>(SystemType::Sys_Graphics));

	return true;
}
int Engine::Draw(Context& context)
{
	Graphic* graph = getSystem<Graphic>(SystemType::Sys_Graphics);
	if (graph == nullptr)
		return false;

	graph->BeginDraw();

	//Draw our game;
	RENDERER->SetColor(Color(1, 0, 0, 1));
	RENDERER->FillRect(100, 100, 300, 200);

	graph->EndDraw();

	return true;
}
int Engine::Update(Context& context)
{
	for (std::pair<SystemType, System*> pSys : m_mapSystems)
	{
		if (!pSys.second)
			continue; 

		pSys.second->Update(context);
	}


	return true;
}
int Engine::ShutDown()
{
	m_EngineState = EngineState::ShuttingDown;

	for (std::pair<SystemType, System* > psys : m_mapSystems) {
		//if (!psys.second->ShutDown()) {
		//	//Logger::Log("Failed to ShutDown System")
		//}
		SafeDelete(psys.second);
	}
	return true;
}

//private methods
void Engine::CheckResize() 
{
	//Find the window system
	//If window has been found, check if it's valid 
	//Get the resize data from the window
	Window* wnd = getSystem<Window>(SystemType::Sys_Window);
	if (wnd && wnd->getResizeData().mustResize)
	{
		//If we need to Resize
		//find the graphics system in the system map
		//If graphic has been found, check if it's valid
		Graphic* graph = getSystem<Graphic>(SystemType::Sys_Graphics);
		if (graph)
		{
			//Fire the resize method from the graphic class
			//Set the resize data from the window back to false
			graph->OnResize(wnd->getResizeData().newWidth, wnd->getResizeData().newHeight);
			wnd->getResizeData().mustResize = false;

		}
	}
}

int Engine::AddSystem(System* psys)
{
	auto element = m_mapSystems.insert(std::make_pair(psys->getType(),psys));
	if (element.second)
		return true;

		return false; 
}
 
Game* Engine::CreateGame()
{
	if (!AddSystem(new Game(GameData())))
		return nullptr;

	Game* game = getSystem<Game>(SystemType::Sys_Game);
	if (!game)
		return nullptr;

		//if (!game->Initialize())
		//return nullptr;

	return game;
}
#pragma endregion