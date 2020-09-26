#include "StdAfx.h"
#include ".\x2channelmanager.h"

CX2ChannelManager::CX2ChannelManager(void)
{
	m_JoinChannel = NULL;
}

CX2ChannelManager::~CX2ChannelManager(void)
{
	for( int i = 0; i < (int)m_ChannelList.size(); i++ )
	{
		CX2Channel* pChannel = m_ChannelList[i];
		SAFE_DELETE( pChannel );
	}
	m_ChannelList.clear();
}

bool CX2ChannelManager::AddChannel( CX2Channel* pChannel )
{
	for( int i = 0; i < (int)m_ChannelList.size(); i++ )
	{
		CX2Channel* pChannelTemp = m_ChannelList[i];
		if( pChannelTemp->GetID() == pChannel->GetID() )
		{
			return false;
		}
	}
	m_ChannelList.push_back( pChannel );
	return true;
}

bool CX2ChannelManager::JoinChannel( int channelID )
{
	for( int i = 0; i < (int)m_ChannelList.size(); i++ )
	{
		CX2Channel* pChannelTemp = m_ChannelList[i];
		if( pChannelTemp->GetID() == channelID )
		{
			m_JoinChannel = pChannelTemp;
			return true;
		}
	}
	return false;
}






