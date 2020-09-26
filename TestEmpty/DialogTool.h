#pragma once

#include <vector>
using namespace std;

class CDialogTool	: public CKTDXStage
{
public:
	struct DRAW_VERTEX
	{
		D3DXVECTOR4	vPos;	// x, y, z, rhw
		D3DCOLOR    color;	// diffuse color
		float		u, v;	// tu, tv

		enum _FVF
		{
			FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1,
		};
	};

	struct LINE_VERTEX
	{
		D3DXVECTOR4	vPos;
		D3DCOLOR	color;

		enum _FVF
		{
			FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
		};
	};

	struct LINE_POS
	{
		D3DXVECTOR2	vPos;
		D3DXVECTOR2	vSize;
		bool		bIsDraw;
	};

public:
	CDialogTool(void);
	~CDialogTool(void);

	virtual HRESULT OnFrameMove( double fTime, float fElapsedTime );
	virtual HRESULT OnFrameRender();

	virtual HRESULT OnResetDevice();
	virtual HRESULT OnLostDevice();

	virtual bool	MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

private:
	void	DrawFace( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color = 0xffffffff );
	void	DrawLine( float nX, float nY, float nWidth, float nHeight, D3DCOLOR color = 0xffffffff );
	void	TextureFilltering();

private:
	LPDIRECT3DDEVICE9	m_pd3dDevice;
	//CKTDXDeviceTexture*	m_pTexture;
	LPDIRECT3DTEXTURE	m_pTexture;

	LINE_POS	m_sLinePos;	//라인정보

	int			m_RenderStateID;
	D3DXVECTOR2	m_vPos;			//텍스쳐 이동좌표
};
