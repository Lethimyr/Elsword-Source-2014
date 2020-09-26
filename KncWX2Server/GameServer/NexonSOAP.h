//{{ 2012. 04. 02	김민성		넥슨 auth soap
#ifdef SERV_NEXON_AUTH_SOAP

#pragma once

#include "../Common/soap/soapElswordGameSoapProxy.h"


#include "Event.h"
#include "CommonPacket.h"
#include "SystemPacket.h"
#include "ClientPacket.h"
#include "ServerPacket.h"

#include "SubThread.h"
#include "KncException.h"


class KNexonSOAP :public KSubThread
{
	struct KWebMethod
	{
		int m_iSoapAction;
		std::string m_strMethodPath;

		KWebMethod()
		{
			m_iSoapAction = 0;
			m_strMethodPath = std::string();
			m_strMethodPath = "";
		};

		KWebMethod( const KWebMethod& t )
		{
			*this = t;
		};

		KWebMethod& operator=( const KWebMethod& t )
		{
			m_iSoapAction = t.m_iSoapAction;
			m_strMethodPath = t.m_strMethodPath;

			return *this;
		};
	};

	_DeclareException;


public:
#   undef _ENUM
#   define _ENUM( id ) id,
	enum
	{
#   include "NexonSOAP_def.h"
	};

	KNexonSOAP(std::string WsdlFile);
	virtual ~KNexonSOAP();



public:
	enum NEXON_SERVICE_CODE
	{
		NSC_ELSWORD		= 5059,
	};

	enum NEXON_GAME_CODE
	{
		NGC_ELSWORD		= 94224,
	};

	virtual void Run();	// Soap Initialize 를 넣기 위해 재정의함 //

	void SoapInitialize();
	void SoapUninitialize();

	static void AddWebMethod( int iSoapAction, std::string szMethod );
	bool GetWebMethod( int iSoapAction, KWebMethod& kInfo );

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-11-06	// 박세훈
	static	void	SetSoapServerAddressForUser( IN const std::string& strAddr )	{	m_strSoapServerAddressForUser = strAddr;	}
	static	void	SetSoapServerAddressForCoupon( IN const std::string& strAddr )	{	m_strSoapServerAddressForCoupon = strAddr;	}
#endif // SERV_NEXON_COUPON_SYSTEM

protected:
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-10-23	// 박세훈
	int	GetCouponTargetItemCount( IN const UidType iUnitUID
								, IN const int iCouponCardNo
								, OUT int& iTargetItemCount
								) const;

	int	GetCouponTargetItem( IN const UidType iUnitUID
						   , IN const int iCouponCardNo
						   , IN const int iStartRowIndex
						   , IN const int iMaximumRows
						   , OUT std::vector<KCouponBoxTargetItem>& vecTargetItem
						   ) const;
#endif // SERV_NEXON_COUPON_SYSTEM

	//----------------------//
	// Process Event handle //
	//----------------------//
	// derived from KSubThread
	virtual void ProcessEvent( const KEventPtr& spEvent );

	// event handler

	//{{ 2011. 08. 10    김민성    채널링 SOAP
#ifdef SERV_CHANNELING_SOAP
	DECL_ON_FUNC( SOAP_GET_TOONILAND_ID_REQ );
#endif SERV_CHANNELING_SOAP
	//}}

	DECL_ON_FUNC( SOAP_CHECK_NEXON_ID_REQ );

#ifdef	SERV_LOCAL_RANKING_SYSTEM // 적용날짜: 2013-03-27
	DECL_ON_FUNC( SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_READ_REQ );
	DECL_ON_FUNC( SOAP_LOCAL_RANKING_NEXON_ID_CHECK_FOR_WRITE_REQ );
#endif	// SERV_LOCAL_RANKING_SYSTEM

	//{{ 2013. 03. 21	 추천인 시스템 개편 - 김민성
#ifdef SERV_RECOMMEND_LIST_EVENT
	DECL_ON_FUNC( SOAP_CHECK_SAME_USER_REQ );
#endif SERV_RECOMMEND_LIST_EVENT
	//}

#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-06-17	// 박세훈
	DECL_ON_FUNC( SOAP_COUPON_LIST_REQ );
	DECL_ON_FUNC( SOAP_REGIST_COUPON_REQ );
	DECL_ON_FUNC( SOAP_COUPON_TARGET_ITEM_REQ );
#endif // SERV_NEXON_COUPON_SYSTEM

protected:
	ElswordGameSoapProxy *p_nx_soap;

	std::string m_strWsdlPath;

	static std::map< int, KWebMethod >  ms_mapWebMethodPath;

private:
#ifdef SERV_NEXON_COUPON_SYSTEM// 작업날짜: 2013-11-06	// 박세훈
	static	const	std::string	m_strCouponSystemServiceCode;
	static	std::string	m_strSoapServerAddressForUser;
	static	std::string	m_strSoapServerAddressForCoupon;
#endif // SERV_NEXON_COUPON_SYSTEM
};

#endif SERV_NEXON_AUTH_SOAP
//}}