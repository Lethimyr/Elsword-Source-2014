#pragma once

#include <KncSmartPtr.h>
#include "Enum/Enum.h"
#include "Event.h"
#include "CommonPacket.h"


//{{ 2013. 05. 30	최육사	제재 리스트 통합
#ifdef SERV_BLOCK_LIST


class KBlockData
{
public:
	KBlockData();
	KBlockData( const KBlockInfo& kInfo );
    virtual ~KBlockData();

	UidType GetBlockUID() const							{ return m_iBlockUID; }
	char	GetBlockType() const						{ return m_cBlockType; }
	UidType	GetBlockTarget() const						{ return m_iBlockTarget; }
	const std::wstring& GetBlockTargetString() const	{ return m_wstrBlockTarget; }
	void	GetBlockInfo( KBlockInfo& kInfo );
	void	SetBlockInfo( const KBlockInfo& kInfo );
	bool	UpdateAndChanged( const KBlockInfo& kInfo );
	bool	IsActiveBlock() const;

protected:
	UidType				m_iBlockUID;
	char				m_cBlockType;
	UidType				m_iBlockTarget;
	std::wstring		m_wstrBlockTarget;
	CTime				m_tStartTime;
	CTime				m_tEndTime;
};


#endif SERV_BLOCK_LIST
//}}