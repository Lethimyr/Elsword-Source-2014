#pragma once 


namespace GarenaBillingUtil
{
	void		 SetServerGroupIDForNoticeID( unsigned int& uiDestUID_, unsigned int uiSrcUID_ );
	unsigned int ExtractServerGroupIDForNoticeID( unsigned int iUID_ );
	unsigned int ExtractNoticeID( unsigned int iUID_ );
}
