#ifndef _D_OPENSSLRSA_H__
#define _D_OPENSSLRSA_H__
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <../Common/OnlyGlobal/AuthAndBilling/PH/openssl/rsa.h>
#include <../Common/OnlyGlobal/AuthAndBilling/PH/openssl/pem.h>


#include "../Common/OnlyGlobal/AuthAndBilling/PH/Security/targetver.h"

#include <stdio.h>
#include <tchar.h>

#include <windows.h>
#include <string>
#include <vector>

typedef unsigned int uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef std::vector<uint8> dataBuffer_t;
typedef std::string string_t;



class COpensslRSA
{
public:
	COpensslRSA():m_pRsa(NULL){}
	~COpensslRSA()
	{
		/*if(m_pRsa)
		{
			RSA_free(m_pRsa);
			m_pRsa = NULL;
		}*/
	}

public:
	bool Init();
	bool Encrypt(const char * pData , int nDataLen,dataBuffer_t & result); 
	bool Decrypt(const char * pData , int nDataLen,dataBuffer_t & result);
private:
	RSA *m_pRsa;
};

extern COpensslRSA OpensslRSA;

#endif // _D_OPENSSLRSA_H__