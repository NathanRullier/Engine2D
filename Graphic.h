#ifndef _GRAPHIC_H
#define _GRAPHIC_H

//C Runtime header files

//Additionnal header files
#include "System.h"

//#ifndef _LOGGER_H
//#include "Logger.h"
//#endif

#ifndef _2DUTILL_H
#include "d2dutill.h"
#endif

//Forward declaration
class Window;

//Struct
struct GraphicData : public SystemData
{
public :
	GraphicData(Window* wnd = nullptr);

		Window* pWnd;
};
class Graphic :
	public System
{
	friend class Engine;
public :
	//Methods
	HRESULT OnResize(UINT width, UINT height);

	ID2D1HwndRenderTarget* GetRenderTarget() {return m_pRenderTarget ; }
	IWICImagingFactory* GetImageFactory() { return m_pImageFactory; }
	ID2D1SolidColorBrush* GetColorBrush() { return m_pColorBrush; }
	ID2D1Factory* GetD2DFactory() { return m_pD2DFactory; }

protected:
	//Methods
	//Constructor & Destructor
	Graphic(const GraphicData& data);
	virtual ~Graphic();

	//Copy Constructor & Default Assigment Opperator
	Graphic(const Graphic& other);
	Graphic& operator = (const Graphic& tref);

	bool Initialize();
	bool ShutDown();
private:
	//Methods
	//void* operator new (size_t size);
	//void operator delete(void* pdelete);
	
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceDependentResources();

	void DiscardDeviceResources();

	void BeginDraw();
	HRESULT EndDraw();

	//Members
	ID2D1HwndRenderTarget* m_pRenderTarget;
	IWICImagingFactory* m_pImageFactory;
	ID2D1SolidColorBrush* m_pColorBrush;
	ID2D1Factory* m_pD2DFactory;
	
	Window* m_pWindow;
};

#endif //_GRAPHIC_H