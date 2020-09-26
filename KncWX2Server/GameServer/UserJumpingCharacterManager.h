#pragma once

#include "ServerDefine.h"

#ifdef SERV_JUMPING_CHARACTER// 작업날짜: 2013-07-10	// 박세훈

#include <map>
#include <atltime.h>

#include "KncUidType.h"

namespace JumpingCharacter
{
	enum JumpingCharacterResult
	{
		JCR_SUCCEED			= 0,
		JCR_FAILED			= 1,
		JCR_NOT_INIT		= 2,
		JCR_ALREADY_INIT	= 3,
	};

	class KUserJumpingCharacterManager
	{
	public:
		KUserJumpingCharacterManager();
		~KUserJumpingCharacterManager(){}

		bool	IsInit( void ) const	{	return m_bInit;	}
		int		Init( IN const std::map< UidType, std::wstring >& mapJumpingInfo );
		int		Check( IN const UidType& iUnitUID, IN const CTime& tEventBeginDate, IN const CTime& tEventEndDate, IN const bool bAccountEvent ) const;
		int		SetJumpingCharacter( IN const UidType iUnitUID, IN const CTime tRegDate );
		int		ResetJumpingCharacter( IN const UidType iUnitUID );
		int		IsJumpingCharacter( IN const UidType iUnitUID ) const;

	private:
		typedef	std::map< UidType, CTime >	MAP_JIC;	// Jumping Info Container
		bool	m_bInit;
		MAP_JIC	m_mapInfo;
	};
}
#endif // SERV_JUMPING_CHARACTER