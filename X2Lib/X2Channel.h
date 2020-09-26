#pragma once


class CX2Channel
{
	public:
		struct ChannelData
		{
			int			ID;
			wstring		name;
			int			minLevel;
			int			maxLevel;
/*
			ChannelData& operator=( const KEVENT_X2_CHANNEL_INFO& data )
			{
				ID			= data.ID;
				name		= data.name;
				minLevel	= data.minLevel;
				maxLevel	= data.maxLevel;

				return *this;
			}
			*/
		};

	public:
		CX2Channel( ChannelData* pChannelData );
		~CX2Channel(void);

		bool CanJoin( int level );
		int GetID(){ return m_pChannelData->ID; }

	private:
		ChannelData*	m_pChannelData;
};
