#pragma once

#include "cryptopp/randpool.h"

#include "SecurityAssociation.h"
#include "SecureBuffer.h"
#include "SADatabase.h"

namespace KncSecurity
{
    void InitSecurity();
    void ReleaseSecurity();

    CryptoPP::RandomNumberGenerator& GetRandomGenerator();
    KSADatabase& GetSADB();
}