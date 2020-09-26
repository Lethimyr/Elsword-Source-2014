
#include "OpensslRSA.h"
//#include "../Common/OnlyGlobal/AuthAndBilling/PH/openssl/applink.c"
#include "GetAppFolder.h"

#define TOKEN_RSA_PUB_KEY_FILE			"publickey.pem"

#define GWS_RSA_PADDING						RSA_PKCS1_PADDING
#define RSA_PADDING_LENGTH					12

COpensslRSA OpensslRSA;

bool COpensslRSA::Init()
{
	BIO * key = BIO_new(BIO_s_file()); 
	BIO_read_filename(key, (GetAppFolder() + TOKEN_RSA_PUB_KEY_FILE).c_str()); 
	m_pRsa = PEM_read_bio_RSA_PUBKEY(key, NULL, NULL, NULL); 
	BIO_free_all(key);
	
	if(!m_pRsa)
	{
		return false;
	}

	return true;
}

bool COpensslRSA::Encrypt( const char * pData, int nDataLen, dataBuffer_t & dataResult )
{
	int nKeyLen = RSA_size(m_pRsa);
	int nBlockLen = nKeyLen - RSA_PADDING_LENGTH;
	
	int nOffset = 0;
	int nLen = (nDataLen - nOffset) > nBlockLen? nBlockLen: (nDataLen - nOffset);
	while(nLen > 0)
	{
		dataBuffer_t dataBuf;
		dataBuf.resize(nKeyLen);

		int nRet = RSA_private_encrypt(nLen, (const unsigned char*)&pData[nOffset], &dataBuf[0], m_pRsa, GWS_RSA_PADDING);
		if (nRet <= 0)
		{
			return false;
		}
		else 
		{
			dataResult.insert(dataResult.end(), &dataBuf[0], &dataBuf[0]+nRet);

			nOffset += nLen;
			nLen = (nDataLen - nOffset) > nBlockLen? nBlockLen: (nDataLen - nOffset);
		}
	}
	
	return true;
}

bool COpensslRSA::Decrypt( const char * pData, int nDataLen, dataBuffer_t & dataResult )
{

	if(!m_pRsa)
	{
		BIO * key = BIO_new(BIO_s_file()); 
		BIO_read_filename(key, (GetAppFolder() + TOKEN_RSA_PUB_KEY_FILE).c_str()); 
		m_pRsa = PEM_read_bio_RSA_PUBKEY(key, NULL, NULL, NULL); 
		BIO_free_all(key);
	}

	int nKeyLen = RSA_size(m_pRsa);
	
	int nOffset = 0;
	int nLen = (nDataLen - nOffset) > nKeyLen? nKeyLen: (nDataLen - nOffset);
	while(nLen > 0)
	{
		dataBuffer_t dataBuf;
		dataBuf.resize(nKeyLen);

		int nRet = RSA_public_decrypt(nLen, (const unsigned char*)&pData[nOffset], &dataBuf[0], m_pRsa, GWS_RSA_PADDING);
		if (nRet <= 0)
		{
			return false;
		}
		else 
		{
			dataResult.insert(dataResult.end(), &dataBuf[0], &dataBuf[0]+nRet);

			nOffset += nLen;
			nLen = (nDataLen - nOffset) > nKeyLen? nKeyLen: (nDataLen - nOffset);
		}
	}
	
	return true;
}