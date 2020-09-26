#pragma once

class CX2ChannelManager
{
	public:
		CX2ChannelManager(void);
		~CX2ChannelManager(void);

		bool AddChannel( CX2Channel* pChannel );
		bool JoinChannel( int channelID );

	private:
		CX2Channel*				m_JoinChannel;
		vector<CX2Channel*>		m_ChannelList;
};
