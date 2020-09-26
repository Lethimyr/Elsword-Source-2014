#include "StdAfx.h"
#include ".\x2channel.h"

CX2Channel::CX2Channel( ChannelData* pChannelData )
{
	m_pChannelData = pChannelData;
}

CX2Channel::~CX2Channel(void)
{
	SAFE_DELETE( m_pChannelData );
}

bool CX2Channel::CanJoin( int level )
{
	if( m_pChannelData->minLevel <= level
		&& m_pChannelData->maxLevel >= level )
	{
		return true;
	}
	else
	{
		return false;
	}
}





