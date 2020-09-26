#include "StdAfx.h"
#include ".\x2stat.h"


const float CX2Stat::Stat::MAX_INCREASE_HP_RATE			= 0.5f;

CX2Stat::CX2Stat(void)
{
}

CX2Stat::~CX2Stat(void)
{
}

bool CX2Stat::Stat::Verify() const
{
	if( m_fBaseHP.Verify()			== false 

		|| m_fAtkPhysic.Verify()	== false 
		|| m_fAtkMagic.Verify()		== false 

		|| m_fDefPhysic.Verify()	== false 
		|| m_fDefMagic.Verify()		== false )
	{
		g_pKTDXApp->SetFindHacking( true );

		return false;
	}

	return true;
}




