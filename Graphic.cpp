#include "Graphic.h"

#include "Window.h"
#ifndef _DELETEMACROS_H
#include "deletemacros.h"
#endif

//struct 
GraphicData::GraphicData(Window* wnd)
	:SystemData(SystemType::Sys_Graphics)
	,pWnd(wnd)
{}
//ClassGraphic
Graphic::Graphic(const GraphicData& data)
	:System(data)
	,m_pRenderTarget(nullptr)
	,m_pD2DFactory(nullptr)
	,m_pColorBrush(nullptr)
	,m_pImageFactory(nullptr)

	,m_pWindow(data.pWnd)
{
}


Graphic::~Graphic()
{
}

HRESULT Graphic::OnResize(UINT width,UINT height) 
{
	if (m_pRenderTarget)
		return m_pRenderTarget->Resize(D2D1::SizeU(width, height));
	else return S_FALSE;
}

bool Graphic::Initialize()
{
	System::Initialize();

	HRESULT hr = CreateDeviceIndependentResources();
	if (FAILED(hr))
		return false;
	hr = CreateDeviceDependentResources();
	if (FAILED(hr))
		return false;

	//Logger::Log(_T("Graphic is initialized"),LOGTYPE_INFO,false);
	return true;
}
bool Graphic::ShutDown()
{
	System::ShutDown();

	SafeRelease(m_pD2DFactory);
	SafeRelease(m_pColorBrush);
	SafeRelease(m_pImageFactory);
	SafeRelease(m_pRenderTarget);

	return true;

}

//Private methods
HRESULT Graphic::CreateDeviceIndependentResources()
{
	HRESULT hr = S_OK;

	//Create a direct2D factory
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pD2DFactory);
	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&m_pImageFactory);

	return hr;
}
HRESULT Graphic::CreateDeviceDependentResources()
{
	HRESULT hr = S_OK;
	if (!m_pRenderTarget)
	{
		//Get the handle to tye window
		HWND hWnd = m_pWindow->getWindowHandle();

		RECT rc;
		GetClientRect(hWnd, &rc);
		D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

		//Create a Direct2D Render Target
		m_pD2DFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, size), &m_pRenderTarget);
		if (FAILED(hr))
			return hr;
		//Create a brush
		hr = m_pRenderTarget->CreateSolidColorBrush((D2D1::ColorF)D2D1::ColorF::Black, &m_pColorBrush);
		if (FAILED(hr))
			return hr;
	}
	return hr;
}
void Graphic::DiscardDeviceResources()
{
	SafeRelease(m_pRenderTarget);
}
void Graphic::BeginDraw()
{
	m_pRenderTarget->BeginDraw();
	m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));
}
HRESULT Graphic::EndDraw()
{
	return m_pRenderTarget->EndDraw();
}