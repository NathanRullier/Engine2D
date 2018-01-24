#ifndef _GRAPHICSDEVICEMANAGER_H
#define _GRAPHICSDEVICEMANAGERg_H

//Forward Declaration
class Graphic;

//Define
#define GRAPHICSDEVICEMANAGER (GraphicsDeviceMaganager::GetInstance())

class GraphicsDeviceMaganager
{
	friend class Engine;
public:
	~GraphicsDeviceMaganager();

	static GraphicsDeviceMaganager* GetInstance();

	Graphic* GetGraphic() { return m_pGraphic; }

private:
	GraphicsDeviceMaganager();
	static GraphicsDeviceMaganager* m_pInstance;

	void SetGraphic(Graphic* graphic) { m_pGraphic = graphic; }
	Graphic* m_pGraphic;
};

#endif //_GRAPHICSDEVICEMANAGER_H