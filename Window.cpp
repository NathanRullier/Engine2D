#include "Window.h"

//#include "Logger.h"

WindowData::WindowData() 
	:SystemData(SystemType::Sys_Invalid)
	,width(-1)
	,height(-1)
	,bits(-1)
	,windowTitle(_T(""))
	,fullScreen(false)
{
}
WindowData::WindowData(int w, int h, const std::tstring& title, int b, bool fs)
	:SystemData(SystemType::Sys_Window)
	, width(w)
	, height(h)
	, bits(b)
	, windowTitle(title)
	, fullScreen(fs)
{

}

ResizeData::ResizeData() 
	:mustResize(false)
	,newWidth(-1)
	,newHeight(-1)
{

}
ResizeData::ResizeData(bool resize, int nw, int nh)
	:mustResize(resize)
	, newWidth(nw)
	, newHeight(nh)
{

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE) {
		//if the message is WM_CREATE, the lParam contains a pointer to a CREATESTRUCT
		//the CREATESTRUCT contains the "this" pointer from de CreateWindow method
		//the "this" pointer of our app is stored in the create struct pcs->lpCreateParams
		CREATESTRUCT* pCs = (CREATESTRUCT*)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)pCs->lpCreateParams);
	}
	else
	{
		//retrieve the stored "this" pointer
		Window* pWindow = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (pWindow)	return pWindow->HandleEnvent(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleEnvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
	{
		if (!HIWORD(wParam))
			this->Activate();
		else this->DeActivate();

		return 0;
	}
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);

		m_ResizeData.mustResize = true;
		m_ResizeData.newWidth = width;
		m_ResizeData.newHeight = height;

		return 0;
	}
	case WM_DISPLAYCHANGE:
	{
		InvalidateRect(hWnd, NULL, FALSE);

		return 0;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

Window::Window(const WindowData& data)
	:System(data)
	,m_hWindows(NULL)
	,m_hInst(NULL)

	,m_Width(data.width)
	,m_Height(data.height)
	,m_Bits(data.bits)

	,m_mbFullScreen(data.fullScreen)

	,m_Title(data.windowTitle)

	,m_ResizeData()
{
}
Window::~Window()
{
}

bool Window::Initialize()
{
	System::Initialize();

	unsigned int pixelFormat;

	DWORD dwExStyle;
	DWORD dwStyle;

	RECT wndRect;

	wndRect.left = (long)0;
	wndRect.right = (long)m_Width;
	wndRect.top = (long)0;
	wndRect.bottom = (long)m_Height;

	WNDCLASS wndClass;
	std::tstring className = m_Title;

	wndClass.style = CS_DBLCLKS | CS_DROPSHADOW | CS_VREDRAW | CS_OWNDC;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = NULL;
	wndClass.cbWndExtra = NULL;
	wndClass.hInstance = (HINSTANCE)GetModuleHandle(NULL);
	wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = NULL;
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = className.c_str();

	if (!RegisterClass(&wndClass))
	{
		//Logger::log(_T("Failed to register window"), LOGTYPE_ERROR, true);
		return false;
	}
	if (m_mbFullScreen) 
	{
		DEVMODE dmScreenSettings;
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = m_Width;
		dmScreenSettings.dmPelsHeight = m_Height;
		dmScreenSettings.dmBitsPerPel = m_Bits;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			//Logger::Log(_T("The Requested Fullscreen Mode Is Not Supported By\nYour Video Card."), LOGTYPE_WARNING, true);
			
			dwExStyle = WS_EX_APPWINDOW;
			dwStyle = WS_POPUP;
			//ShowCurse(FALSE);
		}
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		dwStyle = WS_OVERLAPPEDWINDOW;
	}

	AdjustWindowRectEx(&wndRect, dwStyle, FALSE, dwExStyle);

	if (!(m_hWindows = CreateWindowEx(
		dwExStyle,
		className.c_str(),
		className.c_str(),
		dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		wndRect.right - wndRect.left,
		wndRect.bottom - wndRect.top,
		NULL,
		NULL,
		m_hInst,
		this)))
	{
		//Logger::Log(_T("Failed to window handle = NULL"), LOGTYPE_ERROR, true);
		return false; 
	}
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		m_Bits,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0,
	};
	if (!(m_hDC = GetDC(m_hWindows)))
	{
		//Logger::Log(_T("Can't create Device Context"), LOGTYPE_ERROR, true);
		return false;
	}
	if (!(pixelFormat = ChoosePixelFormat(m_hDC, &pfd)))
	{
		//Logger::Log(_("Can't Find Suitable PixelFormat"), LOGTYPE_ERROR, true);
		return false;
	}
	if (!SetPixelFormat(m_hDC, pixelFormat, &pfd))
	{
		//Logger::Log(_T("Can't Set The PixelFormat"), LOGTYPE_ERROR, true);
		return false;
	}

	ShowWindow(m_hWindows, SW_SHOW);
	SetForegroundWindow(m_hWindows);
	SetFocus(m_hWindows);
	UpdateWindow(m_hWindows);

	m_ResizeData.mustResize = true;
	m_ResizeData.newHeight = m_Height;
	m_ResizeData.newWidth = m_Width;

	if (!this->CenterWindow())
	{
		//Logger::Log(_T("Failed to center window"), LOGTYPE_ERROR, true);
		return false;
	}

	HWND hConsoleWnd = GetConsoleWindow();
	if (hConsoleWnd != NULL)
	{
		HMENU hConsoleMenu = GetSystemMenu(hConsoleWnd, FALSE);
		if (hConsoleMenu != 0)
		{
			BOOL bRet = RemoveMenu(hConsoleMenu, SC_CLOSE, MF_BYCOMMAND);
			if (!bRet)
			{
				//Logger::Log(_T("Failed To Remove Close Button"),LOGTYPE_ERROR,true);
				return false;
			}
		}
	}
	//Logger::Log(_T("Window is initialized"),LOGTYPE_INFO,false);

	//Success
	return true;
}
bool Window::Update(Context& context)
{
	if (WINDOW != this)
		WINDOW = this;

		return true;
}
bool Window::ShutDown()
{
	if (m_mbFullScreen)
	{
		ChangeDisplaySettings(NULL, 0);
		ShowCursor(true);
	}
	
	//Are we able to release the DC
	//Set DC to null
	if (m_hDC && !ReleaseDC(m_hWindows, m_hDC))
	{
		//Logger::Log(_T("Release DC failed"));
		m_hDC = NULL;
		return false;
	}

	//Are we able to release the DC
	//Set DC to null
	if (m_hWindows && !DestroyWindow(m_hWindows))
	{
		//Logger::Log(_T("Could not destroy the window"));
		m_hWindows = NULL;
		return false;
	}

	//Are we able to release the DC
	//Set DC to null
	if (!UnregisterClass(m_Title.c_str(),m_hInst))
	{
		//Logger::Log(_T("Could not unregister class"));
		m_hInst = NULL;
		return false;
	}

	return true;
}
bool Window::CenterWindow()
{
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	DWORD dwStyle = WS_OVERLAPPEDWINDOW;

	RECT R = { 0,0,m_Width,m_Height };

	AdjustWindowRectEx(&R, dwStyle, false, dwExStyle);

	int windowWidth = R.right - R.left;
	int windowHeight = R.bottom - R.top;

	int posx = GetSystemMetrics(SM_CXSCREEN) / 2 - windowWidth / 2;
	int posy = GetSystemMetrics(SM_CYSCREEN) / 2 - windowHeight / 2;

	if (SetWindowPos(m_hWindows, NULL, posx, posy, windowWidth, windowHeight, SWP_SHOWWINDOW) == NULL)
		return false;

	return true;
}