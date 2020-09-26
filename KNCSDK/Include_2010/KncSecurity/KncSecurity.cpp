#include "KncSecurity.h"
#include "cryptopp/osrng.h"
#include "SADatabase.h"
#include "SecureBuffer.h"

#define SAFE_DELETE( x ) if( x != NULL ) { delete x; x = NULL; }

NAMESPACE_BEGIN( KncSecurity )

using namespace CryptoPP;

RandomNumberGenerator*  g_pkRng( NULL );
KSADatabase*            g_pkSADB( NULL );

void InitSecurity()
{
    if( !g_pkRng )  g_pkRng     = new AutoSeededRandomPool;
    if( !g_pkSADB ) g_pkSADB    = new KSADatabase;
}

void ReleaseSecurity()
{
    SAFE_DELETE( g_pkRng );
    SAFE_DELETE( g_pkSADB );
}

CryptoPP::RandomNumberGenerator& GetRandomGenerator()
{
    if( !g_pkRng ) g_pkRng = new AutoSeededRandomPool;

    return *g_pkRng;
}

KSADatabase& GetSADB()
{
    if( !g_pkSADB )
	{
		g_pkSADB    = new KSADatabase();
	}

    return *g_pkSADB;
}

NAMESPACE_END