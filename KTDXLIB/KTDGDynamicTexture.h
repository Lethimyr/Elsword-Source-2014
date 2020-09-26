#pragma once

#include <d3dx9.h>
#include <d3d9.h>

class CKTDGDynamicTexture
{
public:
	CKTDGDynamicTexture(void);
	~CKTDGDynamicTexture(void);

	bool CreateTexture(DWORD dwWidth, DWORD dwHeight, D3DFORMAT d3dFormat);
	
//#ifdef DYNAMIC_VERTEX_BUFFER_OPT
	void SetChange( const RECT& rect );
//#else
//	void SetChange(bool bChange) { m_bUpdateTexture = bChange; }
//#endif
	void ResetDevice();
	void LostDevice();
	void SetTexture(int iStage = 0);

public:
	D3DLOCKED_RECT		m_LockedRect;

private:
	LPDIRECT3DTEXTURE9	m_pTextureSys;
	LPDIRECT3DTEXTURE9	m_pTextureVid;
	
	bool				m_bUpdateTexture;

	DWORD				m_dwWidth, m_dwHeight;
	D3DFORMAT			m_d3dFormat;
};
