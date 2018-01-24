#include "GraphicsDeviceMaganager.h"

#include "Graphic.h"	

GraphicsDeviceMaganager* GraphicsDeviceMaganager::m_pInstance = nullptr;

GraphicsDeviceMaganager::GraphicsDeviceMaganager()
{
}


GraphicsDeviceMaganager::~GraphicsDeviceMaganager()
{
}

GraphicsDeviceMaganager* GraphicsDeviceMaganager::GetInstance()
{
	if (m_pInstance == nullptr)
		m_pInstance = new GraphicsDeviceMaganager;

	return m_pInstance;
}