#pragma once

#include <d3d9.h>

#define SETVECTOR4(vector,_x,_y,_z,_w) {vector.x = _x; vector.y = _y; vector.z = _z; vector.w = _w;}
#define SETVECTOR3(vector,_x,_y,_z) {vector.x = _x; vector.y = _y; vector.z = _z;}
#define SETVECTOR2(vector,_x,_y) {vector.x = _x; vector.y = _y;}
#define SETCOLOR(color,_r,_g,_b,_a) { color.r = _r; color.g = _g; color.b = _b; color.a = _a;}

struct COLLBOXVERTEX
{
	enum _FVF
	{
		FVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1,
	};

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vNormal;
	float       m_fTu, m_fTv;
};

struct SLVERTEX
{
	enum _FVF
	{
		FVF = D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1,
	};

	D3DXVECTOR3 m_vPos;
	DWORD   	m_dwDiff;
	float       m_fTu, m_fTv;

	void SetVertexZ( float x, float y, float z, float fTu, float fTv )
	{
		SETVECTOR3( m_vPos, x, y, z );
		m_dwDiff = 0xffffffff;
		m_fTu = fTu;
		m_fTv = fTv;
	}

	void SetVertex(int x, int y, D3DCOLOR diff, float fTu, float fTv )
	{
		SETVECTOR3( m_vPos, (float)x, (float)y, 0.5f );
		m_dwDiff = diff;
		m_fTu = fTu;
		m_fTv = fTv;
	}

	void SetVertexColor( float x, float y, D3DCOLOR diff, float fTu, float fTv )
	{
		SETVECTOR3( m_vPos, x, y, 0.5f );
		m_dwDiff = diff;
		m_fTu = fTu;
		m_fTv = fTv;
	}
};

struct S2DUIVertex
{
	D3DXVECTOR4 m_vPos; // x, y, z, 1/w
	D3DCOLOR    m_diff; // diffuse color
	D3DXVECTOR2 m_vTex; // tu, tv

	S2DUIVertex( void )
	{
		SETVECTOR4( m_vPos, 0.0f, 0.0f, 0.0f, 1.0f );
		m_diff = D3DCOLOR_ARGB( 0xff, 0xff, 0xff, 0xff );
		SETVECTOR2( m_vTex, 0.0f, 0.0f );
	}
	S2DUIVertex( D3DXVECTOR4& vPos, D3DCOLOR& diff, D3DXVECTOR2& vTex )
	{
		m_vPos = vPos;
		m_diff = diff;
		m_vTex = vTex;
	}

	S2DUIVertex( const S2DUIVertex& src )
	{
		m_vPos = src.m_vPos;
		m_diff = src.m_diff;
		m_vTex = src.m_vTex;
	}
	const S2DUIVertex& operator=( const S2DUIVertex& src )
	{
		m_vPos = src.m_vPos;
		m_diff = src.m_diff;
		m_vTex = src.m_vTex;
		return *this;
	}

	enum _FVF
	{
		FVF = D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1,
	};

	void SetVertexZ( float x, float y, float z, float fTu, float fTv )
	{
		SETVECTOR4( m_vPos, x, y, z, 1.0f );
		m_diff = 0xffffffff;
		m_vTex.x = fTu;
		m_vTex.y = fTv;
	}

	void SetVertex(int x, int y, D3DCOLOR diff, float fTu, float fTv )
	{
		SETVECTOR4( m_vPos, (float)x, (float)y, 0.0f, 1.0f );
		m_diff = diff;
		m_vTex.x = fTu;
		m_vTex.y = fTv;
	}

	void SetVertexColor( float x, float y, D3DCOLOR diff, float fTu, float fTv )
	{
		SETVECTOR4( m_vPos, x, y, 0.0f, 1.0f );
		m_diff = diff;
		m_vTex.x = fTu;
		m_vTex.y = fTv;
	}
};