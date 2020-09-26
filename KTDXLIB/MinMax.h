// MinMax.h: interface for the CMinMax class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINMAX_H__2AD7E04C_7547_48A8_AA0D_6CE46D01770C__INCLUDED_)
#define AFX_MINMAX_H__2AD7E04C_7547_48A8_AA0D_6CE46D01770C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

int		RandomNumber(int iMin, int iMax);
float	RandomNumber(float fMin, float fMax);

inline D3DXVECTOR4 RandomNumber(const D3DXVECTOR4& vMin, const D3DXVECTOR4& vMax)
{
	float x = RandomNumber(vMin.x, vMax.x);
	float y = RandomNumber(vMin.y, vMax.y);
	float z = RandomNumber(vMin.z, vMax.z);
	float w = RandomNumber(vMin.w, vMax.w);
	return(D3DXVECTOR4(x,y,z,w));
}

inline D3DXVECTOR3 RandomNumber(const D3DXVECTOR3& vMin, const D3DXVECTOR3& vMax)
{
  float x = RandomNumber(vMin.x, vMax.x);
  float y = RandomNumber(vMin.y, vMax.y);
  float z = RandomNumber(vMin.z, vMax.z);
  return(D3DXVECTOR3(x,y,z));
}

inline D3DXVECTOR2 RandomNumber(const D3DXVECTOR2& vMin, const D3DXVECTOR2& vMax)
{
	float x = RandomNumber(vMin.x, vMax.x);
	float y = RandomNumber(vMin.y, vMax.y);
	return(D3DXVECTOR2(x,y));
}

inline D3DXCOLOR RandomNumber(D3DXCOLOR Min, D3DXCOLOR Max)
{
  float r = RandomNumber(Min.r, Max.r);
  float g = RandomNumber(Min.g, Max.g);
  float b = RandomNumber(Min.b, Max.b);
  float a = RandomNumber(Min.a, Max.a);
  return(D3DXCOLOR(r,g,b,a));
}

template <class T> class CMinMax
{
	public:
		CMinMax()				{ m_Min = T(); m_Max = T(); }
		CMinMax(T tMin, T tMax) { m_Min = tMin; m_Max = tMax; }
		~CMinMax()				{}

		T m_Min;
		T m_Max;

		T GetRandomNumInRange(void) { return(RandomNumber(m_Min, m_Max)); }
		T GetRange(void)			{ return(abs(m_Max-m_Min)); }

		CMinMax& operator=( const T& data )
		{
			m_Min = data;
			m_Max = data;

			return *this;
		}	
};

#endif // !defined(AFX_MINMAX_H__2AD7E04C_7547_48A8_AA0D_6CE46D01770C__INCLUDED_)
