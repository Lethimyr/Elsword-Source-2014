#include "GarenaBillingUtil.h"




namespace GarenaBillingUtil
{
	void SetServerGroupIDForNoticeID( unsigned int& uiDestUID_, unsigned int uiSrcUID_ )
	{
		uiDestUID_ = ( uiDestUID_ & 0x00ffffff ) | ( ( ( uiSrcUID_ & ( unsigned int )0x000000ff ) << 24 ) & 0xff000000 );
	}

	unsigned int ExtractServerGroupIDForNoticeID( unsigned int iUID_ )
	{
		return ( iUID_ >> 24 ) & ( unsigned int )0x000000ff;
	}

	unsigned int ExtractNoticeID( unsigned int iUID_ )
	{
		return ( iUID_ ) & ( unsigned int )0x00ffffff;
	}
}
