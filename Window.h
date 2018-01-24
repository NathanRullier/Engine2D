#ifndef _WINDOW_H
#define _WINDOW_H

#include "System.h"

//C Runtime header files
#ifndef _WINDOWS_
#include <Windows.h>
#endif

//Additionnal include files
#ifndef _CONTEXT_H
#include "context.h"
#endif
#ifndef _STRING_H
#include "string.h"
#endif

//Structs
struct WindowData :public SystemData 
{
	WindowData();
	WindowData(int w, int h, const std::tstring& title= _T("2DEngine V1.0"), int b=32, bool fs=false);

	int width;
	int height;

	int bits;

	std::tstring windowTitle;

	bool fullScreen;
};

struct ResizeData {
	ResizeData();
	ResizeData(bool resize, int nw, int nh);

	bool mustResize;

	int newWidth;
	int newHeight;
};

class Window :
	public System
{
	friend class Engine;
public:

	int getWidth();
	int getHeight();

	HWND getWindowHandle() { return m_hWindows; }
	HDC getDeviceContext() { return m_hDC; }
	HINSTANCE getInstance() { return m_hInst; }

	ResizeData& getResizeData() { return m_ResizeData; }

	LRESULT HandleEnvent(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

protected:
	Window(const WindowData& data);
	virtual ~Window();

	virtual bool Initialize();
	virtual bool Update(Context& context);
	virtual bool ShutDown();

private :

	bool CenterWindow();

	LRESULT CALLBACK Window_Procedure(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);
	
	//Members
	HWND m_hWindows;
	HDC m_hDC; 
	HINSTANCE m_hInst;

	int m_Width;
	int m_Height;

	int m_Bits;

	bool m_mbFullScreen;
	ResizeData m_ResizeData;

	std::tstring m_Title;
};
#endif