// Precompiled Header
#include "stdafx.h"

#include <WinSock2.h>
#include <Mmsystem.h>

#pragma comment ( lib, "winmm.lib" )

#include "KNetutil.h"

KImplementGlobalVal(KNetUtil);

KNetUtil::KNetUtil()
{
	WSAStartUp();
}

KNetUtil::~KNetUtil()
{
	WSACleanUp();
}

std::string KNetUtil::GetHostName()
{
	char strTemp[256];
	::gethostname( strTemp, 256 );
	return strTemp;
}

std::string KNetUtil::GetHostIP()
{
	char strTemp[256];
	::gethostname( strTemp, 256 );

	HOSTENT* hostent;
	char* ip;

	hostent = ::gethostbyname( strTemp );

	ip = ::inet_ntoa( *( struct in_addr* )*hostent->h_addr_list );
	return ip;
}

int KNetUtil::GetRandomPort()
{
	return 5000 + (timeGetTime() % 10000);
}

void KNetUtil::WSAStartUp()
{
	WSADATA WSAData;
	if ( 0 != WSAStartup( MAKEWORD(2, 2), &WSAData ) )
	{
		return;
	}
}

void KNetUtil::WSACleanUp()
{
	WSACleanup();
}

