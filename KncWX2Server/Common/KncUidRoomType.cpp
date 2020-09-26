#undef max
#include <boost/random.hpp>
#include <Thread/Locker.h>

#include "KncUidRoomType.h"


//namespace KncRoomType 
//{
//	DWORD GetChannelID( IN DWORD iUID_ )
//	{
//		return ( iUID_ >> 24 ) & ( DWORD )0x000000ff;
//	}
//	void SetChannelID( OUT DWORD& iUID_, IN DWORD nChannelID )
//	{
//		iUID_		= iUID_ & 0x00ffffff;
//		nChannelID = ( nChannelID << 24 ) & 0xff000000;
//		iUID_		= iUID_ | nChannelID;
//	}
//
//	DWORD GetRoomType( IN DWORD iUID_ )
//	{
//		return ( iUID_ >> 24 ) & ( DWORD )0x000000ff;
//	}
//	void SetRoomType( OUT DWORD& iUID_, IN DWORD nRoomType )
//	{
//	}
//
//	DWORD GetGameType( IN DWORD iUID_ )
//	{
//	}
//	void SetGameType( OUT DWORD& iUID_, IN DWORD nGameType )
//	{
//	}
//
//	DWORD GetMaxLevel( IN DWORD iUID_ )
//	{
//	}
//	void SetMaxLevel( OUT DWORD& iUID_, IN DWORD nMaxLevel )
//	{
//	}
//
//	DWORD GetMinLevel( IN DWORD iUID_ )
//	{
//	}
//	void SetMinLevel( OUT DWORD& iUID_, IN DWORD nMinLevel )
//	{
//	}
//}