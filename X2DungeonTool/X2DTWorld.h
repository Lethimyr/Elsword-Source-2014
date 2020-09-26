#pragma once

class KX2DTWorld : public CX2World
{
public:	
	KX2DTWorld( WorldData* pWorldData );
	virtual ~KX2DTWorld(void);

	virtual HRESULT		OnFrameRender();

	bool OpenScriptFile( const WCHAR* pFileName );	

	bool GetRenderingWorld()				{ return m_bRenderingWorld; }
	bool GetRenderingLineMap()				{ return m_bRenderingLineMap; }

	void SetRenderingWorld( bool bVal )		{ m_bRenderingWorld = bVal; }
	void SetRenderingLineMap( bool bVal )	{ m_bRenderingLineMap = bVal; }	

private:
	bool		m_bRenderingWorld;
    bool		m_bRenderingLineMap;
};