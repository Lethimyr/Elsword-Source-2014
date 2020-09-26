#pragma once
#include "GiantBaseManager.h"
#include "GiantAuthPacket.h"

#ifdef SERV_COUNTRY_CN

#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_hash_map.h>


class KGiantAuthManager : public KGiantBaseManager
{
	DeclDump;
	DeclToStringW;
	DeclPfID;
	DeclareSingleton( KGiantAuthManager );

protected:
	KGiantAuthManager();

public:
	virtual ~KGiantAuthManager();

	virtual void RegToLua();

	// derived from KThreadManager
	virtual KThread* CreateThread();


	struct RequestInfo
	{
		RequestInfo() 
		{}

		RequestInfo(UidType server_uid, UidType user_uid, std::wstring wstrIP)
			: ServerUID(server_uid)
			, UserUID(user_uid)
			, wstrIP(wstrIP)
		{}

		UidType		ServerUID;
		UidType		UserUID;
		std::wstring wstrIP;
	};

	int RegisterRequest(const RequestInfo& info);
	bool UnregisterRequest(int iRequestID, OUT RequestInfo& info);


protected:
	
	void MakeEventFromReceivedPacket();


	virtual void Handle_OnCommonPacket(KGiantCommonPacket& kReceivedPacket);
	void Handle_OnLoginPacket(KGiantAuthPacket& kReceivedPacket);

#ifdef SERV_DIRECT_CHARGE_ELSWORD_CASH
	void Handle_OnDirectChargePacket(KGiantAuthPacket& kReceivedPacket);
#endif // SERV_DIRECT_CHARGE_ELSWORD_CASH


	unsigned int CreateRequestID();
	void ReleaseRequestID(unsigned int iRequestID);

	
protected:
	struct IntHashCompare
	{
		static size_t hash( const int& x )
		{
			return static_cast<size_t>(x);
		}

		static bool equal( const int& x, const int& y )
		{
			return x == y;
		}
	};

	tbb::concurrent_queue< unsigned int >				m_qRequestID;
	typedef tbb::concurrent_hash_map< unsigned int, RequestInfo, IntHashCompare > RequestMap;
	RequestMap											m_mapRequest;

};

DefSingletonInline( KGiantAuthManager );


#endif // SERV_COUNTRY_CN